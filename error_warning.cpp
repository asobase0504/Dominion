#include "error_warning.h"
#include <assert.h>
#include <d3dx9.h>

void ErrorWarningStatement()
{
	MessageBox(NULL, TEXT("ファイルが開きませんでした"), TEXT("FILE"), MB_ICONHAND);
	assert(false);
}
