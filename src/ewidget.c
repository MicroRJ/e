/*
**
**                      -+- e -+-
**
**                      An Editor?
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**
**            [ TIMENTE MORTEM, OMNIA TIMEO ]
**
**
**                 github.com/MicroRJ/e
**
**
**                 USE AT YOUR OWN RISK
**
*/

int
Emu_widget_render(Emu_widget_t *widget, rlWindowBox rect)
{
  	int in_rect = cursor_in_rect(rect);

  	eeditor_t *editor = widget;

	if (widget->cursor_blink_speed_in_seconds == 0) {
		widget->cursor_blink_speed_in_seconds = .500;
	}

	rxFont *font = editor->font;

	eeditor_msg(editor);

	double animation = .5 + .5 * sin(rxPI_F * widget->cursor_blink_timer / widget->cursor_blink_speed_in_seconds);

	widget->cursor_blink_timer += rx.delta_seconds;

	/* todo */
	rxcolor_t color = RX_RGBA_UNORM(148,232,148,255);
	color.a = rxclamp(animation,.1,.8);

	for(int i=0;i<ccarrlen(editor->cursor);i+=1)
	{
		rlWindowBox cur_rec = ecurrec(editor,i,rect);

		/* this function should just take an e-rect */
		Emu_imp_rect_sdf(
		  rxvec2_xy(
		    cur_rec.x0 + (cur_rec.x1 - cur_rec.x0) * .5,
		    cur_rec.y0 + (cur_rec.y1 - cur_rec.y0) * .5),
		  rxvec2_xy(
		    .5*(cur_rec.x1-cur_rec.x0),
		    .5*(cur_rec.y1-cur_rec.y0)) , color, 2.5, 1.);
	}

	ebuffer_t buffer = editor->buffer;

   Emu_font_text_config_t config;
   ZeroMemory(&config,sizeof(config));
   config.font = font;
   config.x = rect.x0;
   config.y = rect.y1 - font->line_height;
   config.char_height = font->char_height;
   config.line_height = font->line_height;
   config.tab_size = 2; /* in spaces */
   config.color = RX_RGBA(0,0,0,0);
   config.color_table = buffer.syntax.color_table;
   config.length = buffer.length;
   config.string = buffer.memory;
   config.colors = buffer.colors;
   config.line_array = buffer.lcache + editor->lyview;
   config.line_count = (rect.y1 - rect.y0) / editor->font->line_height + 1;

   if(config.line_count > ccarrlen(buffer.lcache)) {
      config.line_count = ccarrlen(buffer.lcache);
   }

   Emu_draw_text( &config );

   return ccfalse;
}














