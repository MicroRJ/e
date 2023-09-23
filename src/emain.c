#define _RX_DEFAULT_WINDOW_SIZE_X 1080 + 256
#define _RX_DEFAULT_WINDOW_SIZE_Y 720
#define _RX_REFRESH_RATE          30

#include <e.h>

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	rl_initWindowed(L"e");

	rlFont *font = rlFont_loadFromFile( "assets\\Hack\\Hack_v3_003\\Hack-Regular.ttf", 22);

	Editor editor;
	ZeroMemory(&editor,sizeof(editor));

	eaddcur(&editor,(ecursor_t){0,0});
	editor.font = font;

	eeditor_load(&editor,"todo.txt");

  // rlTexture *glyphTexture = editor.font.glyphAtlas;

	int debug_overlay = 0;
	int searching = 0;

	do
	{

#if 1
		if(rlIO_testKey(rx_kKEY_F5))
		{
			RunCommandString("build.msvc.template.bat e TRUE");
		} else
		if(rlIO_testKey(rx_kKEY_F1))
		{
			eeditor_unload(&editor,editor.buffer.tag);
		} else
		if(rlIO_testKey(rx_kKEY_F7))
		{ debug_overlay = !debug_overlay;
		} else
		if(rlIO_testKey(rx_kKEY_F9))
		{
		} else
		if(rlIO_testCtrlKey() && rlIO_testMenuKey() && rlIO_testShiftKey() && rlIO_testKey(rx_kKEY_UP))
		{
		} else
		if(rlIO_testCtrlKey() && rlIO_testMenuKey() && rlIO_testShiftKey() && rlIO_testKey(rx_kKEY_DOWN))
		{
		} else
		if(rlIO_testCtrlKey() && rlIO_testKey('P'))
		{
			searching = !searching;
		} else
		if(rlIO_testCtrlKey() && rlIO_testKey('O'))
		{ eeditor_load(&editor,fdlg());
		} else
		if(rlIO_testCtrlKey() && rlIO_testKey('S'))
		{ eeditor_unload(&editor,fdlg());
		} else
		{
			boxthing r = rlGetWindowClientBox();
			rlDrawRect(r,RX_RGBA_UNORM(8,36,36,255));

			if(Editor_render(&editor,r))
			{
			}

			for(int i=0; i<1; i+=1)
			{
				boxthing f = rlBoxCut(&r,boxen_bot,22.);
				rlDrawRect(f,RX_RGBA_UNORM(122,104,81,255));

				draw_single_text_line(editor.font,f.x0,f.y0,
				ccformat("%i,%i %s (%i/%i) %s @%f %f",
				egetcurx(&editor,0),egetcury(&editor,0),
				editor.buffer.tag,
				editor.buffer.length,
				editor.buffer.extent,
				ccFN_makeFileName(editor.font->fpath),editor.font->char_height,1./rx.delta_seconds));
			}
		}
#endif

    /* todo: this is something that rx should do intrinsically */
		Sleep(8);

	} while(rlTick());

	return 0;
}


