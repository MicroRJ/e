#define _RX_DEFAULT_WINDOW_SIZE_X 1080 + 256
#define _RX_DEFAULT_WINDOW_SIZE_Y 720
#define _RX_REFRESH_RATE          30

#include <e.h>

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	rxInitWindowed(L"e");

	rxFont *font = rxLoadFontFile( "assets\\Hack\\Hack_v3_003\\Hack-Regular.ttf", 22);

	eeditor_t editor;
	ZeroMemory(&editor,sizeof(editor));

	eaddcur(&editor,(ecursor_t){0,0});
	editor.font = font;

	eeditor_load(&editor,"todo.txt");

  // Emu_texture_t *glyphTexture = editor.font.glyphAtlas;

	int debug_overlay = 0;
	int searching = 0;

	do
	{

#if 1
		if(rxTestKey(rx_kKEY_F5))
		{
			RunCommandString("build.msvc.template.bat e TRUE");
		} else
		if(rxTestKey(rx_kKEY_F1))
		{
			eeditor_unload(&editor,editor.buffer.tag);
		} else
		if(rxTestKey(rx_kKEY_F7))
		{ debug_overlay = !debug_overlay;
		} else
		if(rxTestKey(rx_kKEY_F9))
		{
		} else
		if(rxIsCtrlKey() && rxIsMenuKey() && rxIsShiftKey() && rxTestKey(rx_kKEY_UP))
		{
		} else
		if(rxIsCtrlKey() && rxIsMenuKey() && rxIsShiftKey() && rxTestKey(rx_kKEY_DOWN))
		{
		} else
		if(rxIsCtrlKey() && rxTestKey('P'))
		{
			searching = !searching;
		} else
		if(rxIsCtrlKey() && rxTestKey('O'))
		{ eeditor_load(&editor,fdlg());
		} else
		if(rxIsCtrlKey() && rxTestKey('S'))
		{ eeditor_unload(&editor,fdlg());
		} else
		{
			rlWindowBox r = rlGetWindowClientBox();
			draw_rect(r,RX_RGBA_UNORM(8,36,36,255));

			if(Emu_widget_render(&editor,r))
			{
			}

			for(int i=0; i<1; i+=1)
			{
				rlWindowBox f = rlBoxCut(&r,RECT_kBOT,22.);
				draw_rect(f,RX_RGBA_UNORM(122,104,81,255));

				draw_single_text_line(editor.font,f.x0,f.y0,
				ccformat("%i,%i %s (%i/%i) %s @%f %f",
				egetcurx(&editor,0),egetcury(&editor,0),
				editor.buffer.tag,
				editor.buffer.length,
				editor.buffer.extent,
				ccfnames(editor.font->fpath),editor.font->char_height,1./rx.delta_seconds));
			}
		}
#endif

    /* todo: this is something that rx should do intrinsically */
		Sleep(8);

	} while(rlTick());

	return 0;
}


