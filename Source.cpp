#include <Windows.h>
#include "resource.h"
#include <CommDlg.h>
#include <fstream>
#include <commctrl.h>

using namespace std;

struct usuario {
	usuario* antU;
	char nomUsuario[30];
	char nomCompleto[50];
	char comercio[8];
	char contra[10];
	char foto[300];
	char tipoUsu[15];
	usuario* sigU;
};

struct cliente {
	cliente* antCl;
	char nomCliente[30];
	char correo[30];
	char comercioCli[8];
	char fecha[30];
	cliente* sigCl;
};

struct promocion {
	promocion* antP;
	char nomProm[30];
	char estatus[9];
	char comercioProm[8];
	char montoProm[30];
	char porcProm[30];
	char fechaProm[30];
	promocion* sigP;
};

struct consumo {
	consumo* antC;
	char clienteConsumo[30];
	char montoCons[30];
	char fechaCons[30];
	char subtotal[30];
	char descuento[30];
	char total[8];
	char comercioC[8];
	consumo* sigC;
};

usuario* auxU, * inicioU, * miUsu = nullptr;
cliente* auxCl, * inicioCl, * cli= nullptr;
promocion* auxP, * inicioP, * prom = nullptr;
consumo* auxC, * inicioC, * cons = nullptr;

LRESULT CALLBACK procVInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVMiUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistroMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVlistaUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVModContra(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVAltaCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVListaCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVAltaProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVListaProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVAltaCons(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVListaCons(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void registrarUsuarios(usuario* nuevoU);
void modificarUsuarios(usuario* newUsu, char nomUsu[30]);
void modificarContrasena(usuario* newContra, char nomUsu[30]);
void BuscarUsuarioRepetido(char usuario[50]);
void eliminarUsuarios(char nomUsuEli[50]);
void escribirUsuarios();
void leerUsuarios();

void registrarClientes(cliente* nuevoCl);
void modificarClientes(cliente* newCli, char nomCli[30]);
void eliminarClientes(char nomCliEli[50]);
void escribirClientes();
void leerClientes();

void registrarPromociones(promocion* nuevoP);
void modificarPromociones(promocion* newProm, char nomP[30]);
void eliminarPromociones(char nomPromEli[50]);
void escribirPromociones();
void leerPromociones();

void registrarConsumos(consumo* nuevoC);
void escribirConsumos();
void leerConsumos();

HINSTANCE hInstGlobal;
char zFile[MAX_PATH];
HWND hPrinGlob;
int usuariorepetido;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	hInstGlobal = hInst;
	leerUsuarios();
	leerClientes();
	leerPromociones();
	leerConsumos();
	HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(101), NULL, procVInicio);

	ShowWindow(hVInicio, cmdshow);

	MSG Mensaje;
	ZeroMemory(&Mensaje, sizeof(Mensaje));

	while (GetMessage(&Mensaje, 0, 0, 0)) {
		TranslateMessage(&Mensaje);
		DispatchMessage(&Mensaje);
	}
	escribirUsuarios();
	escribirClientes();
	escribirPromociones();
	escribirConsumos();
	return Mensaje.wParam;
}

LRESULT CALLBACK procVInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_OKInicio: {
			char nomUsuario[30];
			char contra[30];
			GetDlgItemText(hwnd, ID_UsuInicio, nomUsuario, sizeof(nomUsuario));
			GetDlgItemText(hwnd, ID_contraInicio, contra, sizeof(contra));

			auxU = inicioU;
			if (inicioU == nullptr) {
				MessageBox(hwnd, "No hay usuario registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (auxU != nullptr && strcmp(auxU->nomUsuario, nomUsuario) != 0) {
					auxU = auxU->sigU;
				}
				if (auxU == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (strcmp(auxU->contra, contra) == 0) {
						miUsu = auxU;
						
						EndDialog(hwnd, 0);
						DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_PrincipalMenu), NULL, procVPrincipal);
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}

		}break;
		case ID_BotonRegistro: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_Registro), hwnd, procVRegistro);

		}break;
		case ID_CancelarInicio: {
			int op = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (op) {
			case IDYES: {
				DestroyWindow(hwnd);
			}break;
			}
		}break;
		}
	}break;

	case WM_CLOSE: {
		int op = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		if (op == IDYES) {
			DestroyWindow(hwnd);
		}
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;

	}
	return false;
}

LRESULT CALLBACK procVRegistro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	hPrinGlob = hwnd;

	switch (msg) {
	case WM_INITDIALOG: {
		char tipoUsu[2][15] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, ID_tipoUsuRegistro, CB_INSERTSTRING, (WPARAM)i, (LPARAM)tipoUsu[i]);
		}
		SendMessage(GetDlgItem(hwnd, ID_tipoUsuRegistro), CB_SETCURSEL, (WPARAM)0, 0);

		char comercio[2][15] = { "+KOTA", "Petco" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, ID_ComRegistro, CB_INSERTSTRING, (WPARAM)i, (LPARAM)comercio[i]);
		}
		SendMessage(GetDlgItem(hwnd, ID_ComRegistro), CB_SETCURSEL, (WPARAM)0, 0);

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case  ID_CancelarRegistro: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_InicioSesion), NULL, procVInicio);
		}break;
		case ID_OKRegistro: {
			usuario* newUsu = new usuario;
			GetDlgItemText(hwnd, ID_usuRegistro, newUsu->nomUsuario, sizeof(newUsu->nomUsuario));
			GetDlgItemText(hwnd, ID_ContraRegistro, newUsu->contra, sizeof(newUsu->contra));
			GetDlgItemText(hwnd, ID_ComRegistro, newUsu->comercio, sizeof(newUsu->comercio));
			GetDlgItemText(hwnd, ID_tipoUsuRegistro, newUsu->tipoUsu, sizeof(newUsu->tipoUsu));

			BuscarUsuarioRepetido(newUsu->nomUsuario);
			if (usuariorepetido == 1)
			{
				MessageBox(hwnd, "Nombre de usuario repetido favor de ingresar otro", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			}
			else if (usuariorepetido == 0)
			{
				strcpy_s(newUsu->foto, zFile);
				strcpy_s(newUsu->nomCompleto, "");
				registrarUsuarios(newUsu);
				delete newUsu;

				SetDlgItemText(hwnd, ID_usuRegistro, "");
				SetDlgItemText(hwnd, ID_ContraRegistro, "");
				SetDlgItemText(hwnd, ID_ContraRegistro, "");
			}
		}break;
		}
	}break;

	}
	return false;
}

LRESULT CALLBACK procVPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		if (strcmp(miUsu->tipoUsu, "Cajero") == 0) {
			EnableMenuItem(GetMenu(hwnd), ID_USUARIOS_ALTA, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), ID_PROMOCIONES_ALTA, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), ID_USUARIOS_MODIFICARCONTRASENA, MF_DISABLED | MF_BYCOMMAND);
		}
		auxU = inicioU;

		while (auxU != nullptr) {
			if (strcmp(miUsu->comercio, auxU->comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1078), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
			}
			auxU = auxU->sigU;
		}
		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta2[11] = { 0 };
		char charFecha[5] = { 0 };
		
		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta2, charFecha);
		strcat_s(fechaCompleta2, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta2, charFecha);
		strcat_s(fechaCompleta2, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta2, charFecha);
		
		SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)fechaCompleta2);
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_USUARIOS_MICUENTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_miUsu), hwnd, procVMiUsu);
		}break;
		case ID_USUARIOS_ALTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_RegistroMenu), hwnd, procVRegistroMenu);
		}break;
		case ID_USUARIOS_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_listaUsuMenu), hwnd, procVlistaUsu);
	    }break;
		case ID_USUARIOS_MODIFICARCONTRASENA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ModificarContra), hwnd, procVModContra);
		}break;
		case ID_CLIENTES_ALTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_AC), hwnd, procVAltaCli);
		}break;
		case ID_CLIENTES_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ListaCli), hwnd, procVListaCli);
		}break;
		case ID_PROMOCIONES_ALTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_AP), hwnd, procVAltaProm);
		}break;
		case ID_PROMOCIONES_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ListaProm), hwnd, procVListaProm);
		}break;
		case ID_CONSUMOS_ALTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ACO), hwnd, procVAltaCons);
		}break;
		case ID_CONSUMOS_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_LCO), hwnd, procVListaCons);
		}break;
		case ID_SalirMenu: {
			int op = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (op) {
			case IDYES: {
				DestroyWindow(hwnd);
			}break;
			}
		}break;
		}
	}break;
	case WM_CLOSE: {
		int op = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		if (op == IDYES) {
			DestroyWindow(hwnd);
		}
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	}

	return 0;
}

LRESULT CALLBACK procVAltaCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		
		SYSTEMTIME st;
		SendDlgItemMessage(hwnd, IDC_Fecha, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);

		char comercioCli[2][15] = { "+KOTA", "Petco" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_ComAC, CB_INSERTSTRING, (WPARAM)i, (LPARAM)comercioCli[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_ComAC), CB_SETCURSEL, (WPARAM)0, 0);
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_RegistrarAC: {
			cliente* newCli = new cliente;
			GetDlgItemText(hwnd, IDC_NomAC, newCli->nomCliente, sizeof(newCli->nomCliente));
			GetDlgItemText(hwnd, IDC_CorreoAC, newCli->correo, sizeof(newCli->correo));
			GetDlgItemText(hwnd, IDC_ComAC, newCli->comercioCli, sizeof(newCli->comercioCli));
			GetDlgItemText(hwnd, IDC_Fecha, newCli->fecha, sizeof(newCli->fecha));

			
			registrarClientes(newCli);
			delete newCli;

			SetDlgItemText(hwnd, IDC_NomAC, "");
			SetDlgItemText(hwnd, IDC_CorreoAC, "");
			SetDlgItemText(hwnd, IDC_ComAC, "");
			SetDlgItemText(hwnd, IDC_Fecha, "");
		}break;
		case ID_CancelarAC: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVRegistroMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		
		char tipoUsu[2][15] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_TipoRM, CB_INSERTSTRING, (WPARAM)i, (LPARAM)tipoUsu[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_TipoRM), CB_SETCURSEL, (WPARAM)0, 0);
		char comercio[2][8] = { "+KOTA", "Petco" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_ComRM, CB_INSERTSTRING, (WPARAM)i, (LPARAM)comercio[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_ComRM), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_CancelarRM: {
			EndDialog(hwnd, 0);
		}break;
		case ID_OKRM: {
			usuario* newUsu = new usuario;
			GetDlgItemText(hwnd, IDC_UsuRM, newUsu->nomUsuario, sizeof(newUsu->nomUsuario));
			GetDlgItemText(hwnd, IDC_ContraRM, newUsu->contra, sizeof(newUsu->contra));
			GetDlgItemText(hwnd, IDC_ComRM, newUsu->comercio, sizeof(newUsu->comercio));
			GetDlgItemText(hwnd, IDC_TipoRM, newUsu->tipoUsu, sizeof(newUsu->tipoUsu));
			
			strcpy_s(newUsu->foto, zFile);
			strcpy_s(newUsu->nomCompleto, "");

			registrarUsuarios(newUsu);
			delete newUsu;

			SetDlgItemText(hwnd, IDC_UsuRM, "");
			SetDlgItemText(hwnd, IDC_ContraRM, "");
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVMiUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_contraMiUsu, miUsu->contra);
		SetDlgItemText(hwnd, IDC_miUsuNomCompleto, miUsu->nomCompleto);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);
		
		HBITMAP bmp; 
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); 

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_modificarUsu: {
			EnableWindow(GetDlgItem(hwnd, IDC_miUsuNomCompleto), true);
			EnableWindow(GetDlgItem(hwnd, IDC_contraMiUsu), true);
			EnableWindow(GetDlgItem(hwnd, IDC_cambiarFoto), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RegistrarMiUsu), true);
		}break;
		case IDC_RegistrarMiUsu: {
			GetDlgItemText(hwnd, IDC_miUsuNomCompleto, miUsu->nomCompleto, sizeof(miUsu->nomCompleto));
			GetDlgItemText(hwnd, IDC_contraMiUsu, miUsu->contra, sizeof(miUsu->contra));
			strcpy_s(miUsu->foto, zFile);
			EnableWindow(GetDlgItem(hwnd, IDC_miUsuNomCompleto), false);
			EnableWindow(GetDlgItem(hwnd, IDC_cambiarFoto), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RegistrarMiUsu), false);
			EnableWindow(GetDlgItem(hwnd, IDC_contraMiUsu), false);
		}break;
		case IDC_cambiarFoto: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; 

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); 
				SetDlgItemText(hwnd, IDC_STATICPath, zFile);
				SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);  

			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case IDC_regresarMU: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return 0;
}

LRESULT CALLBACK procVlistaUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		if (strcmp(miUsu->tipoUsu, "Cajero") == 0) {
			EnableWindow(GetDlgItem(hwnd, IDC_EliminarUsu), false);
			EnableWindow(GetDlgItem(hwnd, IDC_modificarUsu), false);
		}
		
		auxU = inicioU;

		while (auxU != nullptr) {
			if (strcmp(miUsu->comercio, auxU->comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1078), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
			}
			auxU = auxU->sigU;
		}
		char comercio[2][8] = { "+KOTA", "Petco" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_EDIT3, CB_INSERTSTRING, (WPARAM)i, (LPARAM)comercio[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_EDIT3), CB_SETCURSEL, (WPARAM)0, 0);

		char tipoUsu[2][15] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_EDIT4, CB_INSERTSTRING, (WPARAM)i, (LPARAM)tipoUsu[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_EDIT4), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_listBoxUsu) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textoList[50];
				int indice = 0;

				indice = SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETTEXT, indice, (LPARAM)textoList);

				auxU = inicioU;
				while (auxU != nullptr && strcmp(textoList, auxU->nomUsuario) != 0) {
					auxU = auxU->sigU;
				}

				if (strcmp(textoList, auxU->nomUsuario) == 0) {
					SetDlgItemText(hwnd, IDC_EDIT1, auxU->nomUsuario);
					SetDlgItemText(hwnd, IDC_EDIT2, auxU->nomCompleto);
					SetDlgItemText(hwnd, IDC_EDIT3, auxU->comercio);
					SetDlgItemText(hwnd, IDC_EDIT4, auxU->tipoUsu);
				}
			}break;
			}
		}
		if (LOWORD(wParam) == IDC_modificarUsu) {
			char textoList[50];
			int indice = 0;
			usuario* newUsu = new usuario;

			indice = SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETTEXT, indice, (LPARAM)textoList);

			GetDlgItemText(hwnd, IDC_EDIT1, newUsu->nomUsuario, sizeof(newUsu->nomUsuario));
			GetDlgItemText(hwnd, IDC_EDIT2, newUsu->nomCompleto, sizeof(newUsu->nomCompleto));
			modificarUsuarios(newUsu, textoList);

			SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_RESETCONTENT, 0, 0);

			auxU = inicioU;
			while (auxU != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
				SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
				auxU = auxU->sigU;
			}
		}
		if (LOWORD(wParam) == IDC_EliminarUsu) {
			char textoList[50];
			int indice = 0;

			indice = SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETTEXT, indice, (LPARAM)textoList);

			eliminarUsuarios(textoList);
			SetDlgItemText(hwnd, IDC_EDIT1, "");
			SetDlgItemText(hwnd, IDC_EDIT2, "");
			SetDlgItemText(hwnd, IDC_EDIT3, "");
			SetDlgItemText(hwnd, IDC_EDIT4, "");

			SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_DELETESTRING, (WPARAM)indice, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_RESETCONTENT, 0, 0);

			auxU = inicioU;
			while (auxU != nullptr) {
				SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
				auxU = auxU->sigU;
			}
		}
		if (LOWORD(wParam) == ID_RegresarLU) {
			EndDialog(hwnd, 0);
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVModContra(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		
		auxU = inicioU;

		while (auxU != nullptr) {
			if (strcmp(miUsu->comercio, auxU->comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1078), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
			}
			auxU = auxU->sigU;
		}
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_listBoxUsu) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textoList[50];
				int indice = 0;

				indice = SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETTEXT, indice, (LPARAM)textoList);

				auxU = inicioU;
				while (auxU != nullptr && strcmp(textoList, auxU->nomUsuario) != 0) {
					auxU = auxU->sigU;
				}

				if (strcmp(textoList, auxU->nomUsuario) == 0) {
					SetDlgItemText(hwnd, IDC_NomUsuMC, auxU->nomUsuario);
					SetDlgItemText(hwnd, IDC_contraMC, auxU->contra);
				}
			}break;
			}
		}
		if (LOWORD(wParam) == IDC_ModificarMC) {
			char textoListMC[50];
			int indiceMC = 0;
			usuario* newContra = new usuario;

			indiceMC = SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_listBoxUsu, LB_GETTEXT, indiceMC, (LPARAM)textoListMC);

			GetDlgItemText(hwnd, IDC_contraMC, newContra->contra, sizeof(newContra->nomUsuario));

			modificarContrasena(newContra, textoListMC);

			SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_RESETCONTENT, 0, 0);
			//SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_RESETCONTENT, 0, 0);

			auxU = inicioU;
			while (auxU != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
				//SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxU->nomUsuario);
				auxU = auxU->sigU;
			}
		}
		if (LOWORD(wParam) == ID_RegresarMC) {
			EndDialog(hwnd, 0);
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVListaCli(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); 
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		
		
		if (strcmp(miUsu->tipoUsu, "Cajero") == 0) {
			EnableWindow(GetDlgItem(hwnd, IDC_EliminarCli), false);
		}
		
		auxCl = inicioCl;
		while (auxCl != nullptr) {
			if (strcmp(miUsu->comercio, auxCl->comercioCli) == 0) {
				SendMessage(GetDlgItem(hwnd, 1086), LB_ADDSTRING, 0, (LPARAM)auxCl->nomCliente);
			}
			auxCl = auxCl->sigCl;
		}
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_ListBoxCli) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textoListCl[50];
				int indiceCl = 0;

				indiceCl = SendDlgItemMessage(hwnd, IDC_ListBoxCli, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_ListBoxCli, LB_GETTEXT, indiceCl, (LPARAM)textoListCl);

				auxCl = inicioCl;
				while (auxCl != nullptr && strcmp(textoListCl, auxCl->nomCliente) != 0) {
					auxCl = auxCl->sigCl;
				}

				if (strcmp(textoListCl, auxCl->nomCliente) == 0) {
					SetDlgItemText(hwnd, IDC_NomLC, auxCl->nomCliente);
					SetDlgItemText(hwnd, IDC_correoLC, auxCl->correo);
					SetDlgItemText(hwnd, IDC_comLC, auxCl->comercioCli);
					SetDlgItemText(hwnd, IDC_FechaLC, auxCl->fecha);
					
				}
			}break;
			}
		}
		if (LOWORD(wParam) == IDC_ModificarCli) {
			char textoListCl[50];
			int indiceCl = 0;
			cliente* newCli = new cliente;

			indiceCl = SendDlgItemMessage(hwnd, IDC_ListBoxCli, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_ListBoxCli, LB_GETTEXT, indiceCl, (LPARAM)textoListCl);

			GetDlgItemText(hwnd, IDC_correoLC, newCli->correo, sizeof(newCli->correo));
			modificarClientes(newCli, textoListCl);

			auxCl = inicioCl;
			while (auxCl != nullptr) {
				SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxCl->correo);
				auxCl = auxCl->sigCl;
			}
		}
		if (LOWORD(wParam) == IDC_EliminarCli) {
			char textoListCl[50];
			int indiceCl = 0;

			indiceCl = SendDlgItemMessage(hwnd, IDC_ListBoxCli, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_ListBoxCli, LB_GETTEXT, indiceCl, (LPARAM)textoListCl);

			eliminarClientes(textoListCl);
			SetDlgItemText(hwnd, IDC_NomLC, "");
			SetDlgItemText(hwnd, IDC_correoLC, "");
			SetDlgItemText(hwnd, IDC_comLC, "");
			SetDlgItemText(hwnd, IDC_FechaLC, "");

			SendMessage(GetDlgItem(hwnd, IDC_listBoxUsu), LB_DELETESTRING, (WPARAM)indiceCl, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_RESETCONTENT, 0, 0);

			auxCl = inicioCl;
			while (auxCl != nullptr) {
				SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)auxCl->nomCliente);
				SendMessage(GetDlgItem(hPrinGlob, IDC_listBoxUsu), LB_ADDSTRING, 0, (LPARAM)"--------------------");
				auxCl = auxCl->sigCl;
			}
		}
		if (LOWORD(wParam) == IDC_RegresarLC) {
			EndDialog(hwnd, 0);
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVAltaProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		SYSTEMTIME st;
		SendDlgItemMessage(hwnd, IDC_FechaProm, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
		char estatus[2][9] = { "Activa", "Inactiva" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_EstatusAP, CB_INSERTSTRING, (WPARAM)i, (LPARAM)estatus[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_EstatusAP), CB_SETCURSEL, (WPARAM)0, 0);

		char comercio[2][8] = { "+KOTA", "Petco" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_ComercioProm, CB_INSERTSTRING, (WPARAM)i, (LPARAM)comercio[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_ComercioProm), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_RegistrarAP: {
			promocion* newProm = new promocion;

			GetDlgItemText(hwnd, IDC_NomPromAP, newProm->nomProm, sizeof(newProm->nomProm));
			GetDlgItemText(hwnd, IDC_MontoAP, newProm->montoProm, sizeof(newProm->montoProm));
			GetDlgItemText(hwnd, IDC_PorcAP, newProm->porcProm, sizeof(newProm->porcProm));
			GetDlgItemText(hwnd, IDC_FechaProm, newProm->fechaProm, sizeof(newProm-> fechaProm));
			GetDlgItemText(hwnd, IDC_EstatusAP, newProm->estatus, sizeof(newProm->estatus));
			GetDlgItemText(hwnd, IDC_ComercioProm, newProm->comercioProm, sizeof(newProm->comercioProm));
			registrarPromociones(newProm);
			delete newProm;
			SetDlgItemText(hwnd, IDC_NomPromAP, "");
			SetDlgItemText(hwnd, IDC_MontoAP, "");
			SetDlgItemText(hwnd, IDC_PorcAP, "");
			
			SetDlgItemText(hwnd, IDC_FechaProm, "");
		}break;
		case ID_CancelarAP: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVListaProm(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		
		if (strcmp(miUsu->tipoUsu, "Cajero") == 0) {
			EnableWindow(GetDlgItem(hwnd, IDC_ModificarProm), false);
			EnableWindow(GetDlgItem(hwnd, IDC_EliminarProm), false);
		}
		
		auxP = inicioP;

		while (auxP != nullptr) {
			if (strcmp(miUsu->comercio, auxP->comercioProm) == 0) {
				SendMessage(GetDlgItem(hwnd, IDC_ListPromNom), LB_ADDSTRING, 0, (LPARAM)auxP->nomProm);
			}
			auxP = auxP->sigP;
		}

		char estatus[2][9] = { "Activa", "Inactiva" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_EstatusLP, CB_INSERTSTRING, (WPARAM)i, (LPARAM)estatus[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_EstatusLP), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_ListPromNom) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textoListP[50];
				int indiceP = 0;

				indiceP = SendDlgItemMessage(hwnd, IDC_ListPromNom, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_ListPromNom, LB_GETTEXT, indiceP, (LPARAM)textoListP);

				auxP = inicioP;
				while (auxP != nullptr && strcmp(textoListP, auxP->nomProm) != 0) {
					auxP = auxP->sigP;
				}

				if (strcmp(textoListP, auxP->nomProm) == 0) {
					SetDlgItemText(hwnd, IDC_NomLP, auxP->nomProm);
					SetDlgItemText(hwnd, IDC_MontoLP, auxP->montoProm);
					SetDlgItemText(hwnd, IDC_PorcLP, auxP->porcProm);
					SetDlgItemText(hwnd, IDC_FechaLP, auxP->fechaProm);
					SetDlgItemText(hwnd, IDC_EstatusLP, auxP->estatus);
					SetDlgItemText(hwnd, IDC_ComercioLP, auxP->comercioProm);
				}
			}break;
			}
		}
		if (LOWORD(wParam) == IDC_ModificarProm) {
			char textoListP[50];
			int indiceP = 0;
			promocion* newProm = new promocion;

			indiceP = SendDlgItemMessage(hwnd, IDC_ListPromNom, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_ListPromNom, LB_GETTEXT, indiceP, (LPARAM)textoListP);

			GetDlgItemText(hwnd, IDC_EstatusLP, newProm->estatus, sizeof(newProm->estatus));


			modificarPromociones(newProm, textoListP);

			SendMessage(GetDlgItem(hwnd, IDC_ListPromNom), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_ListPromNom), LB_RESETCONTENT, 0, 0);

			auxP = inicioP;
			while (auxP != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_ListPromNom), LB_ADDSTRING, 0, (LPARAM)auxP->nomProm);
				SendMessage(GetDlgItem(hPrinGlob, IDC_ListPromNom), LB_ADDSTRING, 0, (LPARAM)auxP->nomProm);
				auxP = auxP->sigP;
			}
		}
		if (LOWORD(wParam) == IDC_EliminarProm) {
			char textoListP[50];
			int indiceP = 0;

			indiceP = SendDlgItemMessage(hwnd, IDC_ListPromNom, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_ListPromNom, LB_GETTEXT, indiceP, (LPARAM)textoListP);

			eliminarPromociones(textoListP);
			SetDlgItemText(hwnd, IDC_NomLP, "");
			SetDlgItemText(hwnd, IDC_PorcLP, "");
			SetDlgItemText(hwnd, IDC_FechaLP, "");
			SetDlgItemText(hwnd, IDC_EstatusLP, "");
			SetDlgItemText(hwnd, IDC_ComercioLP, "");

			SendMessage(GetDlgItem(hwnd, IDC_ListPromNom), LB_DELETESTRING, (WPARAM)indiceP, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_ListPromNom), LB_RESETCONTENT, 0, 0);

			auxP = inicioP;
			while (auxP != nullptr) {
				SendMessage(GetDlgItem(hPrinGlob, IDC_ListPromNom), LB_ADDSTRING, 0, (LPARAM)auxP->nomProm);
				SendMessage(GetDlgItem(hPrinGlob, IDC_ListPromNom), LB_ADDSTRING, 0, (LPARAM)"--------------------");
				auxP = auxP->sigP;
			}
		}
		if (LOWORD(wParam) == IDC_RegresarLP) {
			EndDialog(hwnd, 0);
		}
	}break;
	}
	return false;
}


// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA

LRESULT CALLBACK procVAltaCons(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); //2
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);


		char comercioC[2][8] = { "+KOTA", "Petco" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, IDC_ComercioACO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)comercioC[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_ComercioACO), CB_SETCURSEL, (WPARAM)0, 0);
		SYSTEMTIME st;
		SendDlgItemMessage(hwnd, IDC_FechaACO, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_RegistrarACO: {
			consumo* newCons = new consumo;

			GetDlgItemText(hwnd, IDC_CliNomACO, newCons->clienteConsumo, sizeof(newCons->clienteConsumo));
			GetDlgItemText(hwnd, IDC_MontoACO, newCons->montoCons, sizeof(newCons->montoCons));
			GetDlgItemText(hwnd, IDC_SubTotalACO, newCons->subtotal, sizeof(newCons->subtotal));
			GetDlgItemText(hwnd, IDC_DescuentoACO, newCons->descuento, sizeof(newCons->descuento));
			GetDlgItemText(hwnd, IDC_TotalACO, newCons->total, sizeof(newCons->total));
			GetDlgItemText(hwnd, IDC_FechaACO, newCons->fechaCons, sizeof(newCons->fechaCons));
			GetDlgItemText(hwnd, IDC_ComercioACO, newCons->comercioC, sizeof(newCons->comercioC));

			registrarConsumos(newCons);
			delete newCons;
			SetDlgItemText(hwnd, IDC_CliNomACO, "");
			SetDlgItemText(hwnd, IDC_MontoACO, "");
			SetDlgItemText(hwnd, IDC_SubTotalACO, "");
			SetDlgItemText(hwnd, IDC_DescuentoACO, "");
			SetDlgItemText(hwnd, IDC_TotalACO, "");
			SetDlgItemText(hwnd, IDC_FechaACO, "");
		}break;
		case ID_CancelarACO: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVListaCons(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_usuMiUsu, miUsu->nomUsuario);
		SetDlgItemText(hwnd, IDC_comercioMiUsu, miUsu->comercio);
		SetDlgItemText(hwnd, IDC_tipoMiUsu, miUsu->tipoUsu);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, miUsu->foto, IMAGE_BITMAP, 200, 150, LR_LOADFROMFILE); 
		SendDlgItemMessage(hwnd, ID_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		

		auxC = inicioC;
		while (auxC != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_ListNomCons), LB_ADDSTRING, 0, (LPARAM)auxC->clienteConsumo);
			auxC = auxC->sigC;
		}
		
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_ListNomCons) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textoListCO[50];
				int indiceCO = 0;

				indiceCO = SendDlgItemMessage(hwnd, IDC_ListNomCons, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_ListNomCons, LB_GETTEXT, indiceCO, (LPARAM)textoListCO);

				auxC = inicioC;
				while (auxC != nullptr && strcmp(textoListCO, auxC->clienteConsumo) != 0) {
					auxC = auxC->sigC;
				}

				if (strcmp(textoListCO, auxC->clienteConsumo) == 0) {
					SetDlgItemText(hwnd, IDC_clienteLC, auxC->clienteConsumo);
					//SetDlgItemText(hwnd, IDC_MontoLC, auxC->montoCons);
					SetDlgItemText(hwnd, IDC_DescLC, auxC->descuento);
					SetDlgItemText(hwnd, IDC_SubLC, auxC->subtotal);
					SetDlgItemText(hwnd, IDC_TotaLC, auxC->total);
					SetDlgItemText(hwnd, IDC_FechaLCO, auxC->fechaCons);
					SetDlgItemText(hwnd, IDC_ComercioLCO, auxC->comercioC);

				}
			}break;
			}
		}
		
		if (LOWORD(wParam) == IDC_RegresarLCO) {
			EndDialog(hwnd, 0);
		}
	}break;
	}
	return false;
}

void registrarUsuarios(usuario* nuevoU) {
	if (inicioU == nullptr) { 
		inicioU = new usuario;
		auxU = inicioU;

		auxU->sigU = nullptr;
		auxU->antU = nullptr;

		strcpy_s(auxU->nomUsuario, nuevoU->nomUsuario);
		strcpy_s(auxU->nomCompleto, nuevoU->nomCompleto);
		strcpy_s(auxU->contra, nuevoU->contra);
		strcpy_s(auxU->foto, nuevoU->foto);
		strcpy_s(auxU->comercio, nuevoU->comercio);
		strcpy_s(auxU->tipoUsu, nuevoU->tipoUsu);
		
		
	}
	else {
		auxU = inicioU;

		while (auxU->sigU != nullptr)
		{
			auxU = auxU->sigU;
		}

		auxU->sigU = new usuario;

		auxU->sigU->sigU = nullptr;
		auxU->sigU->antU = auxU;

		auxU = auxU->sigU;

		strcpy_s(auxU->nomUsuario, nuevoU->nomUsuario);
		strcpy_s(auxU->nomCompleto, nuevoU->nomCompleto);
		strcpy_s(auxU->contra, nuevoU->contra);
		strcpy_s(auxU->foto, nuevoU->foto);
		strcpy_s(auxU->comercio, nuevoU->comercio);
		strcpy_s(auxU->tipoUsu, nuevoU->tipoUsu);
		
		
		
	}
	
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
	EndDialog(0, 0);
}

// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA

void modificarUsuarios(usuario* newUsu, char nomUsu[30]) {
	auxU = inicioU;
	char newNom[50];

	if (auxU == nullptr)
	{
		MessageBox(0, "No existe Usuarios registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (auxU != nullptr && strcmp(auxU->nomUsuario, nomUsu) != 0) {
			auxU = auxU->sigU;
		}
		if (auxU == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			strcpy_s(auxU->nomUsuario, newUsu->nomUsuario);
			strcpy_s(auxU->nomCompleto, newUsu->nomCompleto);

			MessageBox(0, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}
}

void eliminarUsuarios(char nomUsuEli[50]) {
	auxU = inicioU;

	if (auxU == nullptr)
	{
		MessageBox(0, "La lista esta vacia. Usuarios", "AVISO", MB_OK);
	}
	else {
		while (auxU != nullptr && strcmp(auxU->nomUsuario, nomUsuEli) != 0) { 

			auxU = auxU->sigU;
		}
		if (auxU == nullptr) {
			MessageBox(0, "No fue encontrado el usuario", "AVISO", MB_OK);
		}
		else if (auxU == inicioU) {	

			if (auxU->sigU == nullptr) { 
				inicioU = nullptr;
				delete auxU;
				auxU = inicioU;
			}
			else { 
				inicioU = inicioU->sigU;
				auxU->sigU->antU = nullptr;
				delete auxU;
				auxU = inicioU;

			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK);
		}
		else {

			if (auxU->sigU == nullptr) { 
				auxU->antU->sigU = nullptr;	
				delete auxU;
				auxU = inicioU;
			}
			else { 
				auxU->sigU->antU = auxU->antU;
				auxU->antU->sigU = auxU->sigU;
				delete auxU;
				auxU = inicioU; 
			}
			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK);
			// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
		}
	}
}

void modificarContrasena(usuario* newContra, char nomContra[30]) {
	auxU = inicioU;

	char newCon[50];

	if (auxU == nullptr)
	{
		MessageBox(0, "No existe Usuarios registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (auxU != nullptr && strcmp(auxU->nomUsuario, nomContra) != 0) {
			auxU = auxU->sigU;
		}
		if (auxU == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			strcpy_s(auxU->contra, newContra->contra);

			MessageBox(0, "Contraseña modificado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}
}

void escribirUsuarios() {
	auxU = inicioU;
	ofstream escribirUsuario; 
	escribirUsuario.open("usuario.bin", ios::out | ios::binary | ios::trunc);

	if (escribirUsuario.is_open()) {
		while (auxU != nullptr)
		{
			escribirUsuario.write((char*)auxU, sizeof(usuario));
			auxU = auxU->sigU;
		}
	}
	escribirUsuario.close();
}

void leerUsuarios() {
	ifstream leerUsuario;
	leerUsuario.open("usuario.bin", ios::in | ios::binary);

	auxU = inicioU;
	if (leerUsuario.is_open()) {

		usuario* usuLeido = new usuario;

		while (!leerUsuario.read((char*)usuLeido, sizeof(usuario)).eof()) {

			while (auxU != nullptr && auxU->sigU != nullptr)
			{
				auxU = auxU->sigU;
			}
			if (auxU == nullptr) {
				inicioU = usuLeido;
				inicioU->sigU = nullptr;
				inicioU->antU = nullptr;
				auxU = inicioU;
			}
			else {
				auxU->sigU = usuLeido;
				auxU->sigU->antU = auxU;
				auxU = auxU->sigU;
				auxU->sigU = nullptr;
			}

			usuLeido = new usuario;
		}
		leerUsuario.close();// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
	}
}

void registrarClientes(cliente* nuevoCl) {
	if (inicioCl == nullptr) {
		inicioCl = new cliente;
		auxCl = inicioCl;

		auxCl->sigCl = nullptr;
		auxCl->antCl = nullptr;

		strcpy_s(auxCl->nomCliente, nuevoCl->nomCliente);
		strcpy_s(auxCl->correo, nuevoCl->correo);
		strcpy_s(auxCl->comercioCli, nuevoCl->comercioCli);
		strcpy_s(auxCl->fecha, nuevoCl->fecha);
	}
	else {
		auxCl = inicioCl;

		while (auxCl->sigCl != nullptr)
		{
			auxCl = auxCl->sigCl;
		}

		auxCl->sigCl = new cliente;

		auxCl->sigCl->sigCl = nullptr;
		auxCl->sigCl->antCl = auxCl;

		auxCl = auxCl->sigCl;

		strcpy_s(auxCl->nomCliente, nuevoCl->nomCliente);
		strcpy_s(auxCl->correo, nuevoCl->correo);
		strcpy_s(auxCl->comercioCli, nuevoCl->comercioCli);
		strcpy_s(auxCl->fecha, nuevoCl->fecha);
	}
	MessageBox(0, "Cliente registrado", "AVISO", MB_OK);
}

void modificarClientes(cliente* newCli, char nomCli[30]) {
	auxCl = inicioCl;
	char newNomCli[50];

	if (auxCl == nullptr)
	{
		MessageBox(0, "No existe clientes registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
	else {
		while (auxCl != nullptr && strcmp(auxCl->nomCliente, nomCli) != 0) {
			auxCl = auxCl->sigCl;
		}
		if (auxCl == nullptr) {
			MessageBox(0, "Cliente no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			strcpy_s(auxCl->correo, newCli->correo);

			MessageBox(0, "Cliente modificado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}
}

void eliminarClientes(char nomCliEli[50]) {
	auxCl = inicioCl;

	if (auxCl == nullptr)
	{
		MessageBox(0, "La lista esta vacia. Agregue Clientes", "AVISO", MB_OK);
	}
	else {
		while (auxCl != nullptr && strcmp(auxCl->nomCliente, nomCliEli) != 0) {

			auxCl = auxCl->sigCl;
		}
		if (auxCl == nullptr) {
			MessageBox(0, "No fue encontrado el cliente", "AVISO", MB_OK);
		}
		else if (auxCl == inicioCl) {

			if (auxCl->sigCl == nullptr) {
				inicioCl = nullptr;
				delete auxCl;
				auxCl = inicioCl;
			}
			else {
				inicioCl = inicioCl->sigCl;
				auxCl->sigCl->antCl = nullptr;
				delete auxCl;
				auxCl = inicioCl;

			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK);
		}
		else {

			if (auxCl->sigCl == nullptr) {
				auxCl->antCl->sigCl = nullptr;
				delete auxCl;
				auxCl = inicioCl;
			}
			else {
				auxCl->sigCl->antCl = auxCl->antCl;
				auxCl->antCl->sigCl = auxCl->sigCl;
				delete auxCl;
				auxCl = inicioCl;
			}
			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK);

		}
	}
}

void escribirClientes() {
	auxCl = inicioCl;
	ofstream escribirCliente;
	escribirCliente.open("cliente.bin", ios::out | ios::binary | ios::trunc);

	if (escribirCliente.is_open()) {
		while (auxCl != nullptr)
		{
			escribirCliente.write((char*)auxCl, sizeof(cliente));
			auxCl = auxCl->sigCl;
		}// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
	}
	escribirCliente.close();
}

void leerClientes() {
	ifstream leerCliente;
	leerCliente.open("cliente.bin", ios::in | ios::binary);

	auxCl = inicioCl;
	if (leerCliente.is_open()) {

		cliente* cliLeido = new cliente;

		while (!leerCliente.read((char*)cliLeido, sizeof(cliente)).eof()) {

			while (auxCl != nullptr && auxCl->sigCl != nullptr)
			{
				auxCl = auxCl->sigCl;
			}
			if (auxCl == nullptr) {
				inicioCl = cliLeido;
				inicioCl->sigCl = nullptr;
				inicioCl->antCl = nullptr;
				auxCl = inicioCl;
			}
			else {
				auxCl->sigCl = cliLeido;
				auxCl->sigCl->antCl = auxCl;
				auxCl = auxCl->sigCl;
				auxCl->sigCl = nullptr;
			}

			cliLeido = new cliente;
		}
		leerCliente.close();
	}
}

void registrarPromociones(promocion* nuevoP) {
	if (inicioP == nullptr) {
		inicioP = new promocion;
		auxP = inicioP;

		auxP->sigP = nullptr;
		auxP->antP = nullptr;

		strcpy_s(auxP->nomProm, nuevoP->nomProm);
		strcpy_s(auxP->estatus, nuevoP->estatus);
		strcpy_s(auxP->montoProm, nuevoP->montoProm);
		strcpy_s(auxP->porcProm, nuevoP->porcProm);
		strcpy_s(auxP->fechaProm, nuevoP->fechaProm);
		strcpy_s(auxP->comercioProm, nuevoP->comercioProm);

	}
	else {
		auxP = inicioP;

		while (auxP->sigP != nullptr)
		{
			auxP = auxP->sigP;
		}

		auxP->sigP = new promocion;

		auxP->sigP->sigP = nullptr;
		auxP->sigP->antP = auxP;

		auxP = auxP->sigP;

		strcpy_s(auxP->nomProm, nuevoP->nomProm);
		strcpy_s(auxP->montoProm, nuevoP->montoProm);
		strcpy_s(auxP->porcProm, nuevoP->porcProm);
		strcpy_s(auxP->fechaProm, nuevoP->fechaProm);
		strcpy_s(auxP->estatus, nuevoP->estatus);
		strcpy_s(auxP->comercioProm, nuevoP->comercioProm);

	}
	MessageBox(0, "Promocion registrada", "AVISO", MB_OK);
}
// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
void modificarPromociones(promocion* newProm, char nomP[30]) {
	auxP = inicioP;
	char newNomProm[50];

	if (auxP == nullptr)
	{
		MessageBox(0, "No existe promociones registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (auxP != nullptr && strcmp(auxP->nomProm, nomP) != 0) {
			auxP = auxP->sigP;
		}
		if (auxP == nullptr) {
			MessageBox(0, "Promocion no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			strcpy_s(auxP->estatus, newProm->estatus);

			MessageBox(0, "Promocion modificada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
	}
}

void eliminarPromociones(char nomPromEli[50]) {
	auxP = inicioP;

	if (auxP == nullptr)
	{
		MessageBox(0, "La lista esta vacia. Agregue promociones", "AVISO", MB_OK);
	}
	else {
		while (auxP != nullptr && strcmp(auxP->nomProm, nomPromEli) != 0) {

			auxP = auxP->sigP;
		}
		if (auxP == nullptr) {
			MessageBox(0, "No fue encontrado la promocion", "AVISO", MB_OK);
		}
		else if (auxP == inicioP) {

			if (auxP->sigP == nullptr) {
				inicioP = nullptr;
				delete auxP;
				auxP = inicioP;
			}
			else {
				inicioP = inicioP->sigP;
				auxP->sigP->antP = nullptr;
				delete auxP;
				auxP = inicioP;

			}

			MessageBox(0, "Promocion eliminada", "AVISO", MB_OK);
		}
		else {

			if (auxP->sigP == nullptr) {
				auxP->antP->sigP = nullptr;
				delete auxP;
				auxP = inicioP;
			}
			else {
				auxP->sigP->antP = auxP->antP;
				auxP->antP->sigP = auxP->sigP;
				delete auxP;
				auxP = inicioP;// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
			}
			MessageBox(0, "Promocion eliminada", "AVISO", MB_OK);

		}
	}
}

void escribirPromociones() {
	auxP = inicioP;
	ofstream escribirPromocion;
	escribirPromocion.open("promocion.bin", ios::out | ios::binary | ios::trunc);

	if (escribirPromocion.is_open()) {
		while (auxP != nullptr)
		{
			escribirPromocion.write((char*)auxP, sizeof(promocion));
			auxP = auxP->sigP;
		}
	}
	escribirPromocion.close();
}

void leerPromociones() {
	ifstream leerPromocion;
	leerPromocion.open("promocion.bin", ios::in | ios::binary);

	auxP = inicioP;
	if (leerPromocion.is_open()) {

		promocion* promLeido = new promocion;

		while (!leerPromocion.read((char*)promLeido, sizeof(promocion)).eof()) {

			while (auxP != nullptr && auxP->sigP != nullptr)
			{
				auxP = auxP->sigP;
			}
			if (auxP == nullptr) {
				inicioP = promLeido;
				inicioP->sigP = nullptr;
				inicioP->antP = nullptr;
				auxP = inicioP;
			}
			else {
				auxP->sigP = promLeido;
				auxP->sigP->antP = auxP;
				auxP = auxP->sigP;
				auxP->sigP = nullptr;
			}

			promLeido = new promocion;
		}
		leerPromocion.close();
	}// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
}

void registrarConsumos(consumo* nuevoC) {
	if (inicioC == nullptr) {
		inicioC = new consumo;
		auxC = inicioC;

		auxC->sigC = nullptr;
		auxC->antC = nullptr;

		strcpy_s(auxC->clienteConsumo, nuevoC->clienteConsumo);
		strcpy_s(auxC->montoCons, nuevoC->montoCons);
		strcpy_s(auxC->fechaCons, nuevoC->fechaCons);
		strcpy_s(auxC->subtotal, nuevoC->subtotal);
		strcpy_s(auxC->descuento, nuevoC->descuento);
		strcpy_s(auxC->total, nuevoC->total);
		strcpy_s(auxC->comercioC, nuevoC->comercioC);
	}
	else {
		auxC = inicioC;

		while (auxC->sigC != nullptr)
		{
			auxC = auxC->sigC;
		}

		auxC->sigC = new consumo;

		auxC->sigC->sigC = nullptr;
		auxC->sigC->antC = auxC;

		auxC = auxC->sigC;

		strcpy_s(auxC->clienteConsumo, nuevoC->clienteConsumo);
		strcpy_s(auxC->montoCons, nuevoC->montoCons);
		strcpy_s(auxC->fechaCons, nuevoC->fechaCons);
		strcpy_s(auxC->subtotal, nuevoC->subtotal);
		strcpy_s(auxC->descuento, nuevoC->descuento);
		strcpy_s(auxC->total, nuevoC->total);
		strcpy_s(auxC->comercioC, nuevoC->comercioC);
	}
	MessageBox(0, "Consumo registrado", "AVISO", MB_OK);
}

void escribirConsumos() {
	auxC = inicioC;
	ofstream escribirConsumo;
	escribirConsumo.open("consumo.bin", ios::out | ios::binary | ios::trunc);

	if (escribirConsumo.is_open()) {
		while (auxC != nullptr)
		{
			escribirConsumo.write((char*)auxC, sizeof(consumo));
			auxC = auxC->sigC;
		}
	}
	escribirConsumo.close();
}// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA

void leerConsumos() {
	ifstream leerConsumo;
	leerConsumo.open("consumo.bin", ios::in | ios::binary);

	auxC = inicioC;
	if (leerConsumo.is_open()) {

		consumo* consLeido = new consumo;

		while (!leerConsumo.read((char*)consLeido, sizeof(promocion)).eof()) {

			while (auxC != nullptr && auxC->sigC != nullptr)
			{
				auxC = auxC->sigC;
			}
			if (auxC == nullptr) {
				inicioC = consLeido;
				inicioC->sigC = nullptr;
				inicioC->antC = nullptr;
				auxC = inicioC;
			}
			else {
				auxC->sigC = consLeido;
				auxC->sigC->antC = auxC;
				auxC = auxC->sigC;// ESTO LO DESCARGARON DE GITHUB PROPIEDAD DE UN ESTUDIANTE DE LMAD QUE YA PASO LA MATERIA
				auxC->sigC = nullptr;
			}

			consLeido = new consumo;
		}
		leerConsumo.close();
	}
}

void BuscarUsuarioRepetido(char usuario[50]) {
	usuariorepetido = 0;
	auxU = inicioU;
	if (inicioU == nullptr) {
		usuariorepetido = 0;
	}
	while (auxU != nullptr && strcmp(auxU->nomUsuario, usuario) != 0) {
		auxU = auxU->sigU;
	}
	if (auxU == nullptr) {
		usuariorepetido = 0;
	}
	else {
		usuariorepetido = 1;
	}
}