#include "error_warning.h"
#include <assert.h>
#include <d3dx9.h>

void ErrorWarningStatement()
{
	MessageBox(NULL, TEXT("�t�@�C�����J���܂���ł���"), TEXT("FILE"), MB_ICONHAND);
	assert(false);
}
