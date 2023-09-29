/*
**
** Copyright(c) 2023 - Dayan Rodriguez - Dyr
**
** -+- e -+-
**
*/

// [ TIMENTE MORTEM, OMNIA TIMEO ]

// sample program using -+- rx -+-

#define _RX_DEFAULT_WINDOW_SIZE_X 1080 + 256
#define _RX_DEFAULT_WINDOW_SIZE_Y 720
#define _RX_REFRESH_RATE          30
#define _RX_STANDALONE
#include <rx/rx.c>


int eeditor_unload(EEditor *editor, char const *name);
void eeditor_load(EEditor *editor, char const *name);

#include <e.h>

# pragma comment(lib,"Comdlg32")
#include   "commdlg.h"

const char *fdlg() {
	char DirBuff[MAX_PATH];
	GetCurrentDirectory(sizeof(DirBuff),DirBuff);

	OPENFILENAMEA OpenFileName;
	ZeroMemory(&OpenFileName,sizeof(OpenFileName));
	OpenFileName.lStructSize     = sizeof(OpenFileName);
	OpenFileName.hwndOwner       = NULL;
	OpenFileName.lpstrFile       = ccstatic_alloc(MAX_PATH,TRUE);
	OpenFileName.nMaxFile        = MAX_PATH;
	OpenFileName.lpstrFilter     = "All\0*.*\0Text\0*.TXT\0";
	OpenFileName.nFilterIndex    = 1;
	OpenFileName.lpstrFileTitle  = NULL;
	OpenFileName.nMaxFileTitle   = 0;
	OpenFileName.lpstrInitialDir = NULL;
	OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileNameA(&OpenFileName);

	SetCurrentDirectory(DirBuff);

	return OpenFileName.lpstrFile;
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	rxinit(L"e");

	rlFont_Face *font = rx_loadFontFromFile( "assets\\Hack\\Hack_v3_003\\Hack-Regular.ttf", 22);

	EEditor editor;
	ZeroMemory(&editor,sizeof(editor));

	eaddcur(&editor,(ECursor){0,0});
	editor.font = font;

	eeditor_load(&editor,"todo.txt");

  // rxGPU_Texture *glyphTexture = editor.font.glyphAtlas;

	int debug_overlay = 0;
	int searching = 0;

	do
	{

#if 1
		if(rx_testKey(rxKEY_kF5))
		{
			RunCommandString("build.msvc.template.bat e TRUE");
		} else
		if(rx_testKey(rxKEY_kF1))
		{
			eeditor_unload(&editor,editor.buffer.tag);
		} else
		if(rx_testKey(rxKEY_kF7))
		{ debug_overlay = !debug_overlay;
		} else
		if(rx_testKey(rxKEY_kF9))
		{
		} else
		if(rx_testCtrlKey() && rx_testAltKey() && rx_testShiftKey() && rx_testKey(rxKEY_kUP))
		{
		} else
		if(rx_testCtrlKey() && rx_testAltKey() && rx_testShiftKey() && rx_testKey(rxKEY_kDOWN))
		{
		} else
		if(rx_testCtrlKey() && rx_testKey('O')) {
			eeditor_load(&editor,fdlg());
		} else
		if(rx_testCtrlKey() && rx_testKey('S')) {
			eeditor_unload(&editor,fdlg());
		} else
		{
		}
		boxthing rect = makeboxthing(0,0,rx.wnd.size_x,rx.wnd.size_y);
		boxthing r = rlGetWindowClientBox();
		rlDrawRect(r,rxColor_RGBA_U(8,36,36,255));
		Editor_render(&editor,r);
#endif

    /* todo: this is something that rx should do intrinsically */
		Sleep(8);

	} while(rxtick());

	return 0;
}



void
eeditor_load(
EEditor *editor, char const *name)
{
	if(name != 0 && strlen(name) != 0)
	{
		void *file = ccopenfile(name,"r");
		ccu32_t length = 0;
		void *memory = ccpullfile(file,0,&length);

		if(memory != 0)
		{
			EBuffer_uninit(&editor->buffer);
			EBuffer_initSized(&editor->buffer,name,length);

			memcpy(editor->buffer.string,memory,length);

			EBuffer_reformat(&editor->buffer);
		}
	}
}

int
eeditor_unload(EEditor *editor, char const *name)
{
	int result = rxFalse;

  /* todo: this should be safer */
	if((name != 0) && (strlen(name) != 0))
	{
		HANDLE file = CreateFileA(name,
		GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ,0x00,CREATE_ALWAYS,0x00,0x00);

		if(file != INVALID_HANDLE_VALUE)
		{
			DWORD length = 0;

      /* todo: convert indentations to white space here? */
			if(WriteFile(file,
			editor->buffer.string,
			editor->buffer.length,&length,0x00))
			{
				result = length == editor->buffer.length;
			}

			CloseHandle(file);
		}
	}


	return result;
}