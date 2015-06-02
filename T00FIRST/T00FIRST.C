#include <stdio.h>
#include <windows.h>

void main( void )
{
  MessageBox(NULL, "Press YES or NO.", "This is a caption", MB_YESNO | MB_ICONQUESTION); 
  printf("Now do it again.\n");
  MessageBox(NULL, "You have no choice.", "This is a caption", MB_OK); 
  if (MessageBox(NULL, "Press YES or NO.", "This is a caption", MB_YESNO| MB_ICONQUESTION) == IDNO)  
    printf("Y U NO\n");    
  else
    printf("AW YISS\n");
}                                                                         