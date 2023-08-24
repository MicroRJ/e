// /*
// **
// **                      -+- e -+-
// **
// **                      An Editor?
// **
// **  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
// **  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
// **  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
// **
// **
// **            [ TIMENTE MORTEM, OMNIA TIMEO ]
// **
// **
// **                 github.com/MicroRJ/e
// **
// **
// **                 USE AT YOUR OWN RISK
// **
// */
// #define VIEW_USAGE

// eheap_2d_config_t
// eheap_config_init(
//   char const *tag, int size_x, int size_y, int x0, int y0, int x1, int y1)
// {
//   eheap_2d_config_t config;
//   config.tag = tag;
//   config.size_x = size_x;
//   config.size_y = size_y;
//   config.x0 = x0;
//   config.y0 = y0;
//   config.x1 = x1;
//   config.y1 = y1;
//   return config;
// }

// int
// eheap_2d_area(
//   eheap_2d_t *heap)
// {
//   return (heap->x1 - heap->x0) * (heap->y1 - heap->y0);
// }

// eheap_2d_t *
// ebank_alloc_heap(
//   ebank_2d_t *bank, eheap_2d_config_t config)
// {
//   eheap_2d_t *result = NULL;

//   if( (config.x1 > config.x0) &&
//       (config.y1 > config.y0) )
//   {
//     result = ccmalloc(sizeof(eheap_2d_t));
//     memset(result,0,sizeof(*result));

//     result->tag = config.tag;
//     result->use = TRUE;

//     result->id = bank->next_id ++;
//     result->x0 = config.x0;
//     result->y0 = config.y0;
//     result->x1 = config.x1;
//     result->y1 = config.y1;
//   }

//   return result;
// }

// void
// ebank_2d_init(
//   ebank_2d_t *bank, int size_x, int size_y)
// {
//   bank->next_id = 0;
//   bank->heap = ebank_alloc_heap(bank,eheap_config_init("root",size_x,size_y,0,0,size_x,size_y));
// }



// /* we could support rotations? #pending */
// int eheap_2d_is_large_enough(
//   eheap_2d_t *heap, int size_x, int size_y)
// {
//   return
//     (heap->x1 - heap->x0) >= size_x &&
//     (heap->y1 - heap->y0) >= size_y;
// }

// eheap_2d_t *
// eheap_2d_query_large_enough_heap(
//   eheap_2d_t *heap, int size_x, int size_y)
// {
//   eheap_2d_t *result = NULL;

//   if(eheap_2d_is_large_enough(heap,size_x,size_y))
//   {
//     if(heap->head != NULL)
//     {
//       result = eheap_2d_query_large_enough_heap(heap->head,size_x,size_y);
//     }

//     if(result == NULL || result->use != TRUE)
//     {
//       if(heap->tail != NULL)
//       {
//         result = eheap_2d_query_large_enough_heap(heap->tail,size_x,size_y);
//       }
//     }

//     if(result == NULL || result->use != TRUE)
//     {
//       if(heap->use != FALSE)
//       {
//         result = heap;
//       }
//     }
//   }


//   return result;
// }

// void
// ebank_fork_heap(
//   ebank_2d_t *bank, eheap_2d_t *heap, int size_x, int size_y)
// {
//   ccassert(heap->head == NULL);
//   ccassert(heap->tail == NULL);

//   int x0,y0,x1,y1;
//   x0 = heap->x0;
//   y0 = heap->y0;
//   x1 = heap->x1;
//   y1 = heap->y1;

//   heap->size_x = size_x;
//   heap->size_y = size_y;
//   heap->use    = FALSE;

//   if((size_x * size_y) < (x1 - x0) * (y1 - y0))
//   {
//     eheap_2d_t *tail = ebank_alloc_heap(bank,eheap_config_init("tail",0,0,x0,y0+size_y,x1,y1));
//     eheap_2d_t *head = ebank_alloc_heap(bank,eheap_config_init("head",0,0,x0+size_x,y0,x1,y0+size_y));

//     heap->head = head;
//     heap->tail = tail;

//     /* #todo */
//     ccassert(head == NULL || size_x == head->x0 - heap->x0);
//     ccassert(tail == NULL || size_y == tail->y0 - heap->y0);

//     /* head must always be smaller than tail */
//     if(head && tail)
//     {
//       if(eheap_2d_area(head) > eheap_2d_area(tail))
//       {
//         heap->head = tail;
//         heap->tail = head;
//       }
//     }
//   }

// }

// int
// ebank_2d_alloc(
//   ebank_2d_t *bank, int size_x, int size_y, int *x, int *y)
// {
//   int result = 0;

//   eheap_2d_t *heap = eheap_2d_query_large_enough_heap(bank->heap,size_x,size_y);

//   if(heap != NULL)
//   {
//     ebank_fork_heap(bank,heap,size_x,size_y);


//     result = heap->id;

//     if(x) *x = heap->x0;
//     if(y) *y = heap->y0;
//   }

//   return result;
// }

// void
// draw_rect(
//   erect_t rect, rxcolor_t color);


// rxcolor_t
// color_mix2(double val, rxcolor_t a, rxcolor_t b)
// {
//   return (rxcolor_t)
//   { rxmix(val, a.x,b.x),
//     rxmix(val, a.y,b.y),
//     rxmix(val, a.z,b.z), 1. };
// }

// rxcolor_t
// color_mix3(double val, rxcolor_t a, rxcolor_t b, rxcolor_t c)
// {
//   return color_mix2(val,color_mix2(val,a,b),color_mix2(val,b,c));
// }


// rxcolor_t
// eheap_get_color(double val)
// {
//   return color_mix3(val,RX_COLOR_RED,RX_COLOR_GREEN,RX_COLOR_BLUE);
// }

// void
// eheap_2d_draw_debug(
//   eheap_2d_t *heap)
// {
//   if(heap == NULL)
//   {
//     return;
//   }

//   rxvec2i_t xy0 = (rxvec2i_t){heap->x0,heap->y0};
//   rxvec2i_t xy1 = (rxvec2i_t){heap->x1,heap->y1};

// #ifdef VIEW_USAGE
//   xy0.x = heap->x0;
//   xy0.y = heap->y0;
//   xy1.x = heap->x0 + heap->size_x;
//   xy1.y = heap->y0 + heap->size_y;
// #endif

//   if(xy1.x != 0 && xy1.y != 0)
//   {
// #ifdef VIEW_USAGE
//     EMU_imp_apply(EMU_IMP_MODE_2D);

//     EMU_imp_begin(6,6);
//       rxaddnvtx(6,
//         rxvtx_xyuv_col(xy0.x,xy0.y, 0,1, RX_COLOR_RED  ),
//         rxvtx_xyuv_col(xy0.x,xy1.y, 0,0, RX_COLOR_WHITE),
//         rxvtx_xyuv_col(xy1.x,xy1.y, 1,0, RX_COLOR_BLUE ),

//         rxvtx_xyuv_col(xy0.x,xy0.y, 0,1, RX_COLOR_RED  ),
//         rxvtx_xyuv_col(xy1.x,xy1.y, 1,0, RX_COLOR_BLUE ),
//         rxvtx_xyuv_col(xy1.x,xy0.y, 1,1, RX_COLOR_GREEN));
//       rxaddnidx(6, 0,1,2, 3,4,5);
//     EMU_imp_end();
// #endif
//     EMU_imp_outline(RX_COLOR_BLACK,
//       xy0.x,xy0.y,
//       xy1.x - xy0.x,
//       xy1.y - xy0.y);

//     // EMU_draw_text_config_t config =
//     // draw_text_config_init(default_font,8,
//     //   xy0.x,xy0.y,RX_COLOR_BLACK,NULL,NULL,-1,
//     //     ccformat("#%i (%s) %i,%i - %i,%i", heap->id,heap->tag,
//     //       heap->x0,heap->y0,
//     //       heap->x1,heap->y1));
//     // edraw_text(&config);
//   }

//   eheap_2d_draw_debug(heap->head);
//   eheap_2d_draw_debug(heap->tail);
// }

// void
// ebank_2d_draw_debug(
//   ebank_2d_t *bank)
// {

//   eheap_2d_draw_debug(bank->heap);
// }
