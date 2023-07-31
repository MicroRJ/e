
int
ecfg_rgb(unsigned int *length, char const **string, rxcolor_t *color)
{
  char const *cursor = *string;

  if((*cursor ++ != 'r') ||
     (*cursor ++ != 'g') ||
     (*cursor ++ != 'b') )
  {
    return ccfalse;
  }

  *color = RX_RGBA(0,0,0,1);

  while(
    (*cursor ==  ' ') ||
    (*cursor == '\t') ||
    (*cursor == '\r') ||
    (*cursor == '\n') ) cursor += 1;

  if((*cursor ++ != '('))
  {
    return ccfalse;
  }

  int rgb[3];

  int i;
  for(i=0;i<3;i+=1)
  {
    while(
      (*cursor ==  ' ') ||
      (*cursor == '\t') ||
      (*cursor == '\r') ||
      (*cursor == '\n') ) cursor += 1;

    estring_to_int(10,&rgb[i],length,&cursor);

    while(
      (*cursor ==  ' ') ||
      (*cursor == '\t') ||
      (*cursor == '\r') ||
      (*cursor == '\n') ) cursor += 1;

    color->e[i] = rgb[i] / 255;

    if(*cursor == ',')
      cursor += 1;
    else
      break;
  }

  if(i != 2)
  {
    return ccfalse;
  }

  if(*cursor ++ != ')')
  {
    return ccfalse;
  }


  return cctrue;
}

int
ecfgfld(unsigned int *length, char const **string, char *label, char *value)
{
  char const *cursor = *string;

  while(
    (*cursor ==  ' ') ||
    (*cursor == '\t') ||
    (*cursor == '\r') ||
    (*cursor == '\n') ) cursor += 1;

  while(cursor < (*string + *length))
  {
    while( (cursor < (*string + *length)) &&
     ( CCWITHIN(*cursor,'A','Z') ||
       CCWITHIN(*cursor,'a','z') ||
       CCWITHIN(*cursor,'_','_') ) ) *label ++ = *cursor ++;

    if( (*cursor == '.') ||
        (*cursor == '/') )
    {
      *label ++ = *cursor ++;
    } else
    if( (*cursor == ':') )
    {
      break;
    } else
    {
      break;
    }
  }

  while(
    (*cursor ==  ' ') ||
    (*cursor == '\t') ||
    (*cursor == '\r') ||
    (*cursor == '\n') ) cursor += 1;

  if(*cursor == '=')
  { cursor += 1;

    while(
      (*cursor ==  ' ') ||
      (*cursor == '\t') ||
      (*cursor == '\r') ||
      (*cursor == '\n') ) cursor += 1;

    // rxcolor_t color;
    ecfg_rgb(length,&cursor,(rxcolor_t*)(value));

    // while( (cursor < (*string + *length)) &&
    //  ( CCWITHIN(*cursor,'A','Z') ||
    //    CCWITHIN(*cursor,'a','z') ||
    //    CCWITHIN(*cursor,'_','_') ) ) *value ++ = *cursor ++;


    *length -= cursor - *string;
    *string = cursor;
    return cctrue;
  }

  *length -= cursor - *string;
  *string = cursor;
  return ccfalse;
}




