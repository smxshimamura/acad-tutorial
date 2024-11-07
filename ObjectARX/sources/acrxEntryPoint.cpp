// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class ARXTrainingApp : public AcRxArxApp {

public:
	ARXTrainingApp() : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void ADSKMyGroupMyCommand () {
		acutPrintf(_T("\r\nHello World!\r\n"));
	}

	static void ADSKMyGroupEX2() {
		const AcGePoint3d pt(50.0, 50.0, 0.0);
		const AcGeVector3d vec(0.0, 0.0, 1.0);
		constexpr double rad = 50.0;
		auto* pCircle = new AcDbCircle(pt, vec, rad);
		auto* pDb = acdbHostApplicationServices()->workingDatabase();

		AcDbBlockTable* pTbl;
		pDb->getBlockTable(pTbl, AcDb::kForRead);

		AcDbBlockTableRecord* pRec;
		pTbl->getAt(MODEL_SPACE, pRec, AcDb::kForWrite);

		pTbl->close();
		pRec->appendAcDbEntity(pCircle);
		pRec->close();
		pCircle->close();
	}

	static void ADSKMyGroupEX3() {
		acedInitGet(RSG_NONULL, nullptr);
		ads_point adsMid;
		auto status = acedGetPoint(nullptr, _T("\nSet start:"), adsMid);
		if (status != RTNORM) return;
		const AcGePoint3d pt(adsMid[X], adsMid[Y], adsMid[Z]);
		acutPrintf(_T("%d, %d\r\n"), pt.x, pt.y);

		ads_point adsEnd;
		status = acedGetPoint(adsMid, _T("\nSet radius:"), adsEnd);
		if (status != RTNORM) return;
		const AcGePoint3d tp(adsEnd[X], adsEnd[Y], adsEnd[Z]);
						
		const double rad = pt.distanceTo(tp);
		acutPrintf(_T("%d\r\n"), rad);
		const AcGeVector3d vec(0.0, 0.0, 1.0);

		auto* pCircle = new AcDbCircle(pt, vec, rad);

		auto* pDb = acdbHostApplicationServices()->workingDatabase();

		AcDbBlockTable* pTbl;
		pDb->getBlockTable(pTbl, AcDb::kForRead);

		AcDbBlockTableRecord* pRec;
		pTbl->getAt(MODEL_SPACE, pRec, AcDb::kForWrite);

		pTbl->close();
		pRec->appendAcDbEntity(pCircle);
		pRec->close();
		pCircle->close();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(ARXTrainingApp)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, MyCommand, MyCommand, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX2, EX2, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX3, EX3, ACRX_CMD_MODAL, NULL)

