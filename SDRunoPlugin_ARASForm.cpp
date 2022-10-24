#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Winsock library

#include <sstream>
//#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
//#endif
#include "SDRunoPlugin_ARASForm.h"

#include "SDRunoPlugin_ARASUi.h"
#include "resource.h"
#include <io.h>
#include <shlobj.h>


#define VERSION "V0.1"
int port = 9876;
string host = "192.168.1.40";
string msg;
WSADATA wsa;
SOCKET sock;
char buffer[10];
// Form constructor with handles to parent and uno controller - launches form Setup
SDRunoPlugin_ARASForm::SDRunoPlugin_ARASForm(SDRunoPlugin_ARASUi& parent, IUnoPluginController& controller) :
	nana::form(nana::API::make_center(default_formWidth, default_formHeight), nana::appearance(false, true, false, false, true, false, false)),
	m_parent(parent),
	m_controller(controller)
	
{
	mode = 0;
	Setup();
}


// Form deconstructor
SDRunoPlugin_ARASForm::~SDRunoPlugin_ARASForm()
{

}


// Start Form and start Nana UI processing
void SDRunoPlugin_ARASForm::Run()
{
	show();
	nana::exec();
	//loaded = true;
	
	//if (m_ui->conn == true)
	//{
	//	SetMode(m_ui->mode);
	//	ChangeStatus("Connected");
	//	SentMsg(m_ui->sent);
	//	RecMsg(m_ui->response);


	//}

}
static bool load_image_from_res(nana::paint::image& img, unsigned short id) {
	HDC hdc = GetDC(NULL);
	if (hdc == NULL)
		return false;

	HMODULE hModule = GetModuleHandle(L"SDRunoPlugin_ARAS");
	if (hModule == NULL)
		return false;

	HBITMAP hBitmap = (HBITMAP)LoadImage(hModule, MAKEINTRESOURCE(id), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	if (hBitmap == NULL)
		return false;

	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);

	if (GetDIBits(hdc, hBitmap, 0, 0, NULL, &bitmapInfo, DIB_RGB_COLORS) == NULL)
		return false;

	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	const unsigned int rawDataOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);

	BITMAPFILEHEADER bitmapFileHeader = { 0 };
	bitmapFileHeader.bfOffBits = rawDataOffset;
	bitmapFileHeader.bfSize = bitmapInfo.bmiHeader.biSizeImage;
	bitmapFileHeader.bfType = 0x4D42;

	std::unique_ptr<BYTE[]> lpPixels = std::make_unique<BYTE[]>(bitmapInfo.bmiHeader.biSizeImage + rawDataOffset);

	*(BITMAPFILEHEADER*)lpPixels.get() = bitmapFileHeader;
	*(BITMAPINFO*)(lpPixels.get() + sizeof(BITMAPFILEHEADER)) = bitmapInfo;

	if (GetDIBits(hdc, hBitmap, 0, bitmapInfo.bmiHeader.biHeight, (LPVOID)(lpPixels.get() + rawDataOffset), &bitmapInfo, DIB_RGB_COLORS) == NULL)
		return false;

	img.open(lpPixels.get(), bitmapInfo.bmiHeader.biSizeImage);

	ReleaseDC(NULL, hdc);

	return true;
}

// Create the initial plugin form
void SDRunoPlugin_ARASForm::Setup()
{
	

	unsigned int formWidth = default_formWidth;
	unsigned int formHeight = default_formHeight;



	// This first section is all related to the background and border
	// it shouldn't need to be changed

	bg_border.size({ formWidth, formHeight });
	bg_inner.size({ formWidth - (2 * sideBorderWidth), formHeight - topBarHeight - bottomBarHeight });
	bg_inner.move({ sideBorderWidth, topBarHeight });

	form_drag_label.size({ formWidth, formHeight });

	versionLbl.move({ (int)(formWidth - 40), (int)(formHeight - 30) });

	nana::paint::image img_border;
	nana::paint::image img_inner;

	load_image_from_res(img_border, IDB_BG_BORDER);
	load_image_from_res(img_inner, IDB_BACKGROUND);
	load_image_from_res(img_close_normal, IDB_CLOSE);
	load_image_from_res(img_close_down, IDB_CLOSE_DOWN);
	load_image_from_res(img_min_normal, IDB_MIN);
	load_image_from_res(img_min_down, IDB_MIN_DOWN);
	load_image_from_res(img_header, IDB_HEADER);


	bg_border.load(img_border, nana::rectangle(0, 0, 590, 340));
	bg_border.stretchable(0, 0, 0, 0);
	bg_border.transparent(true);
	bg_inner.load(img_inner, nana::rectangle(0, 0, 582, 299));
	bg_inner.stretchable(sideBorderWidth, 0, sideBorderWidth, bottomBarHeight);
	bg_inner.transparent(false);

	// TODO: Form code starts here

	// Load X and Y location for the form from the ini file (if exists)
	move(formX_, formY_);
	events().move([&](const nana::arg_move& mov) {
		formX_ = mov.x;
		formY_ = mov.y;
		});

	
	// This code sets the plugin size, title and what to do when the X is pressed
	size(nana::size(formWidth, formHeight));
	caption("SDRuno ARAS Plugin");
	events().destroy([&] { m_parent.FormClosed(); });

	//Initialize header bar
	header_bar.size(nana::size(122, 20));
	header_bar.load(img_header, nana::rectangle(0, 0, 122, 20));
	header_bar.stretchable(0, 0, 0, 0);
	header_bar.move(nana::point((formWidth / 2) - 61, 5));
	header_bar.transparent(true);

	//Initial header text 
	title_bar_label.size(nana::size(65, 12));
	title_bar_label.move(nana::point((formWidth / 2) - 5, 9));
	title_bar_label.format(true);
	title_bar_label.caption("< bold size = 6 color = 0x000000 font = \"Verdana\">A.R.A.S.</>");
	title_bar_label.text_align(nana::align::center, nana::align_v::center);
	
	title_bar_label.fgcolor(nana::color_rgb(0x000000));
	title_bar_label.transparent(true);

	//Iniitialize drag_label
	form_drag_label.move(nana::point(0, 0));
	form_drag_label.transparent(true);

	//Initialize dragger and set target to form, and trigger to drag_label 
	form_dragger.target(*this);
	form_dragger.trigger(form_drag_label);

	//Initialise the "Minimize button"
	min_button.load(img_min_normal, nana::rectangle(0, 0, 20, 15));
	min_button.bgcolor(nana::color_rgb(0x000000));
	min_button.move(nana::point(formWidth - 51, 9));
	min_button.transparent(true);
	min_button.events().mouse_down([&] { min_button.load(img_min_down, nana::rectangle(0, 0, 20, 15)); });
	min_button.events().mouse_up([&] { min_button.load(img_min_normal, nana::rectangle(0, 0, 20, 15)); nana::API::zoom_window(this->handle(), false); });
	min_button.events().mouse_leave([&] { min_button.load(img_min_normal, nana::rectangle(0, 0, 20, 15)); });

	//Initialise the "Close button"
	close_button.load(img_close_normal, nana::rectangle(0, 0, 20, 15));
	close_button.bgcolor(nana::color_rgb(0x000000));
	close_button.move(nana::point(formWidth - 26, 9));
	close_button.transparent(true);
	close_button.events().mouse_down([&] { close_button.load(img_close_down, nana::rectangle(0, 0, 20, 15)); });
	close_button.events().mouse_up([&] { close_button.load(img_close_normal, nana::rectangle(0, 0, 20, 15)); close(); });
	close_button.events().mouse_leave([&] { close_button.load(img_close_normal, nana::rectangle(0, 0, 20, 15)); });



	versionLbl.fgcolor(nana::colors::white);
	versionLbl.caption(VERSION);
	versionLbl.transparent(true);

	// TODO: Extra Form code goes here

	Lbl1.fgcolor(nana::colors::white);
	Lbl1.transparent(true);
	Lbl1.caption("Status");

	Lbl2.fgcolor(nana::colors::white);
	Lbl2.transparent(true);
	Lbl2.caption("Sent");

	Lbl3.fgcolor(nana::colors::white);
	Lbl3.transparent(true);
	Lbl3.caption("Received");


	sLbl.fgcolor(nana::colors::white);
	sLbl.transparent(true);
	sLbl.caption("Disconnected");

	tLbl.fgcolor(nana::colors::white);
	tLbl.transparent(true);
	tLbl.caption("Null");

	rLbl.fgcolor(nana::colors::white);
	rLbl.transparent(true);
	rLbl.caption("Null");

	mLbl.fgcolor(nana::colors::white);
	mLbl.transparent(true);
	mLbl.caption("Normal");
	
	mBtn.bgcolor(nana::colors::white);
	mBtn.fgcolor(nana::colors::black);
	mBtn.transparent(false);
	mBtn.caption("Mode Toggle");
	mBtn.events().click([&] {
		if (mode == 1)
		{
			mode = 0;
			mLbl.caption("Normal");
		}
		else
		{
			mode = 1;
			mLbl.caption("Preamp");
		}
		//ToggleMode(mode);
		});
	cBtn.bgcolor(nana::colors::black);
	cBtn.fgcolor(nana::colors::white);
	cBtn.transparent(true);
	cBtn.enable_focus_color(false);
	cBtn.caption("Connect");
	cBtn.events().click([&] {
		StartConnection();
		//ToggleMode(mode);
		});
	

}




void SDRunoPlugin_ARASForm::ChangeStatus(string status)
{
	sLbl.caption(status);
}
void SDRunoPlugin_ARASForm::SentMsg(string msg)
{
	tLbl.caption(msg);
}
void SDRunoPlugin_ARASForm::RecMsg(string msg)
{
	rLbl.caption(msg);
}
void SDRunoPlugin_ARASForm::SetMode(int md)
{
	mode = md;
	if (mode == 0)
	{
		mLbl.caption("Normal");
		}
	else
	{
		mLbl.caption("Preamp");
	}
	
}
void SDRunoPlugin_ARASForm::StartConnection()
{
	struct hostent* server;
	struct sockaddr_in serv_addr;

	//int port = stoi(cport);


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		ChangeStatus("Socket Error");
		return;
	}


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{

		WSACleanup();
		ChangeStatus("Invalid Socket");

		return;
	}

	server = gethostbyname(host.c_str());
	serv_addr.sin_addr.s_addr = inet_addr(server->h_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);



	// Connect to server
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{

		closesocket(sock);
		WSACleanup();
		ChangeStatus("Connection error");

		return;
	}



	else
	{

		//Connected to socket Server

		//m_form->ChangeStatus("Connected");
		Connected();
	}

}
void SDRunoPlugin_ARASForm::Disconnect()
{
	closesocket(sock);
	WSACleanup();
	
}

void SDRunoPlugin_ARASForm::Connected()
{
	conn = true;
	ChangeStatus("Connected");
	msg = "RF?";
	SendCmd(msg);
	sent = "GetStatus";
	SentMsg(sent);
	status = RecResp();
	RecMsg(status);
	msg = "MODE?";
	SendCmd(msg);
	response = RecResp();

	if (response == "MODE_N")
	{
		mode = 0;

	}
	else
	{
		mode = 1;
	}
	SetMode(mode);
	

}
void SDRunoPlugin_ARASForm::SendCmd(string msg)
{
	send(sock, msg.c_str(), msg.size() + 1, 0);

}
string SDRunoPlugin_ARASForm::RecResp()
{
	string recvd;
	recv(sock, buffer, 10, 0);
	recvd += buffer;
	return recvd;

}
