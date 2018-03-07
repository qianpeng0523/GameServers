#include "StdAfx.h"
#include "LibPOCO.h"

CLibPOCO::CLibPOCO(void)
{
}

CLibPOCO::~CLibPOCO(void)
{
}

void CLibPOCO::Test(int argc, char **argv)
{
	Server app;
	app.run(argc,argv);
}