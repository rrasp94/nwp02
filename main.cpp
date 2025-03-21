#include "nwpwin.h"
#include "res.h"

class button : public vsite::nwp::window {
protected:
	std::string class_name() override { return "BUTTON"; }
};

class edit : public vsite::nwp::window {
protected:
	std::string class_name() override { return "EDIT"; }
};

class list_box : public vsite::nwp::window {
protected:
	std::string class_name() override { return "LISTBOX"; }
};

class main_window : public vsite::nwp::window
{
	edit edt;
	button btn_add, btn_remove;
	list_box lst;

protected:
	int on_create(CREATESTRUCT* pcs) override;
	void on_command(int id) override;
	void on_destroy() override;
};

int main_window::on_create(CREATESTRUCT* pcs)
{
	{
		edt.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, IDC_EDIT, 10, 10, 200, 25);
		lst.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, IDC_LB, 10, 50, 200, 100);
		btn_add.create(*this, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, "Add", IDC_ADD, 220, 10, 100, 30);
		btn_remove.create(*this, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, "Remove", IDC_REMOVE, 220, 50, 100, 30);

		::EnableWindow(btn_remove, FALSE);

		return 0;
	}
	
}

void main_window::on_command(int id){
	switch(id){
		case ID_FILE_EXIT:
			on_destroy();
			break;
		case ID_HELP_ABOUT:
			MessageBox(*this, "Add -> add to list\nRemove -> removes from list.", "Help", MB_OK | MB_ICONINFORMATION);
			break;
		case IDC_ADD:
		{
			char text[256];
			::GetDlgItemText(*this, IDC_EDIT, text, sizeof(text));
			if (strlen(text) > 0)
			{
				::SendDlgItemMessage(*this, IDC_LB, LB_ADDSTRING, 0, (LPARAM)text);
				::EnableWindow(btn_remove, TRUE);
			}
			break;
		}
		case IDC_REMOVE:
		{
			int selected = ::SendDlgItemMessage(*this, IDC_LB, LB_GETCURSEL, 0, 0);
			if (selected != LB_ERR)
			{
				::SendDlgItemMessage(*this, IDC_LB, LB_DELETESTRING, selected, 0);
				int count = ::SendDlgItemMessage(*this, IDC_LB, LB_GETCOUNT, 0, 0);
				if (count == 0)
				{
					::EnableWindow(btn_remove, FALSE);
				}
			}
			break;
		}
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	main_window w; 
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 2", (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_V2)));
	vsite::nwp::set_icons(instance, w, IDI_V2);
	vsite::nwp::application app;
	return app.run();
}
