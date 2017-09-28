#pragma once
#include <atlimage.h>

class CImg : public CImage
{
public:
	CImg(void);
	~CImg(void);

public:
	virtual HRESULT Load( LPCTSTR pszFileName, bool bPreMultiply = true );
};
