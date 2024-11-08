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

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")
using namespace std;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class ARXTrainingApp : public AcRxArxApp {

public:
	virtual ~ARXTrainingApp() = default;
	ARXTrainingApp() {}

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

#pragma region Lesson1

	static void ADSKMyGroupMyCommand () {
		acutPrintf(_T("\r\nHello World!\r\n"));
	}

#pragma endregion

#pragma region Lesson2

	static void ADSKMyGroupEX2() {
		const AcGePoint3d pt(50.0, 50.0, 0.0);
		const AcGeVector3d vec(0.0, 0.0, 1.0);
		constexpr double rad = 50.0;
		const auto pCircle = new AcDbCircle(pt, vec, rad);
		const auto pDb = acdbHostApplicationServices()->workingDatabase();

		AcDbBlockTable* pTbl = nullptr;
		AcDbBlockTableRecord* pRec = nullptr;
		{
			auto stat = pDb->getBlockTable(pTbl, kForRead);
			if (stat != eOk) goto finally;;

			stat = pTbl->getAt(MODEL_SPACE, pRec, kForWrite);
			if (stat != eOk) goto finally;

			pRec->appendAcDbEntity(pCircle);
		}
		finally:
		if (pTbl != nullptr) pTbl->close();
		if (pRec != nullptr) pRec->close();
		
		pCircle->close();
	}

#pragma endregion

#pragma region Lesson3

	static void ADSKMyGroupEX3() {
		AcDbBlockTable* pTbl = nullptr;
		AcDbBlockTableRecord* pRec = nullptr;
		AcDbCircle* pCircle = nullptr;
		{
			acedInitGet(RSG_NONULL, nullptr);
			ads_point adsMid;
			auto res = acedGetPoint(nullptr, _T("\nSet start:"), adsMid);
			if (res != RTNORM) return;
			const AcGePoint3d pt(adsMid[X], adsMid[Y], adsMid[Z]);
			acutPrintf(_T("%d, %d\r\n"), pt.x, pt.y);

			ads_point adsEnd;
			res = acedGetPoint(adsMid, _T("\nSet radius:"), adsEnd);
			if (res != RTNORM) return;
			const AcGePoint3d tp(adsEnd[X], adsEnd[Y], adsEnd[Z]);

			const double rad = pt.distanceTo(tp);
			acutPrintf(_T("%d\r\n"), rad);
			const AcGeVector3d vec(0.0, 0.0, 1.0);

			pCircle = new AcDbCircle(pt, vec, rad);
			const auto pDb = acdbHostApplicationServices()->workingDatabase();

			auto stat = pDb->getBlockTable(pTbl, kForRead);
			if (stat != eOk) goto finally;

			stat = pTbl->getAt(MODEL_SPACE, pRec, kForWrite);
			if (stat != eOk) goto finally;

			pRec->appendAcDbEntity(pCircle);
		}
		finally:
		if (pCircle != nullptr) pCircle->close();
		if (pTbl != nullptr) pTbl->close();
		if (pRec != nullptr) pRec->close();
	}

#pragma endregion

#pragma region Lesson4

	static void ADSKMyGroupEX4() {
		ads_name adsName;
		ads_point adsPt;
		const auto res = acedEntSel(_T("Select object: "), adsName, adsPt);
		if (res != RTNORM) return;

		AcDbObjectId id;
		auto stat = acdbGetObjectId(id, adsName);
		if (stat != eOk) return;

		const auto pTrans = actrTransactionManager->startTransaction();
		AcDbLayerTable* pTbl = nullptr;
		AcDbLayerTableRecord* pLayer = nullptr;
		{
			AcDbEntity* pEnt;
			stat = pTrans->getObject(pEnt, id, AcDb::kForWrite);
			if (stat != eOk) goto finally;

			if (pEnt->isA() != AcDbLine::desc()) {
				acutPrintf(_T("This command supports LINE only."));
				goto finally;
			}

			const auto pDb = acdbHostApplicationServices()->workingDatabase();

			stat = pDb->getLayerTable(pTbl, kForWrite);
			if (stat != eOk) goto finally;

			const auto szName = _T("training");
			if (!pTbl->has(szName)) {
				pLayer = new AcDbLayerTableRecord;
				pLayer->setName(szName);
				if (stat != eOk) goto finally;
				pTbl->add(pLayer);
				if (stat != eOk) goto finally;
			}
			else {
				stat = pTbl->getAt(szName, pLayer, kForWrite);
				if (stat != eOk) goto finally;
			}

			pEnt->setLayer(pLayer->objectId());
		}
		finally:
		if (pLayer != nullptr) pLayer->close();
		if (pTbl != nullptr) pTbl->close();
		actrTransactionManager->endTransaction();
	}

#pragma endregion

#pragma region Lesson5

	static void ADSKprintSymbolName(const AcDbSymbolTable* pTbl) {
		AcDbSymbolTableIterator* pItr;
		auto stat = pTbl->newIterator(pItr);
		if (stat != eOk) return;

		AcDbSymbolTableRecord* pRec;
		for (pItr->start(); !pItr->done(); pItr->step()) {
			stat = pItr->getRecord(pRec, kForRead);
			if (stat != eOk) continue;

			TCHAR* pszName;
			stat = pRec->getName(pszName);
			if (stat == eOk) {
				acutPrintf(_T("  %s\r\n"), pszName);
				free(pszName);
			}
			
			pRec->close();
		}

		delete pItr;
	}

	static void ADSKMyGroupEX5() {
		const auto pDb = acdbHostApplicationServices()->workingDatabase();

		AcDbBlockTable* pBlockTbl;
		auto stat = pDb->getBlockTable(pBlockTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[BlockTable]\r\n"));
			ADSKprintSymbolName(pBlockTbl);
			pBlockTbl->close();
		}

		AcDbDimStyleTable* pDimTbl;
		stat = pDb->getDimStyleTable(pDimTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[DimStyleTable]\r\n"));
			ADSKprintSymbolName(pDimTbl);
			pDimTbl->close();
		}

		AcDbLayerTable* pLayerTbl;
		stat = pDb->getLayerTable(pLayerTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[LayerTable]\r\n"));
			ADSKprintSymbolName(pLayerTbl);
			pLayerTbl->close();
		}

		AcDbLinetypeTable* pLineTbl;
		stat = pDb->getLinetypeTable(pLineTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[LineTypeTable]\r\n"));
			ADSKprintSymbolName(pLineTbl);
			pLineTbl->close();
		}

		AcDbRegAppTable* pAppTbl;
		stat = pDb->getRegAppTable(pAppTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[RegAppTable]\r\n"));
			ADSKprintSymbolName(pAppTbl);
			pAppTbl->close();
		}

		AcDbTextStyleTable* pTextTbl;
		stat = pDb->getTextStyleTable(pTextTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[TextStyleTable]\r\n"));
			ADSKprintSymbolName(pTextTbl);
			pTextTbl->close();
		}

		AcDbUCSTable* pUCSTbl;
		stat = pDb->getUCSTable(pUCSTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[UCSTable]\r\n"));
			ADSKprintSymbolName(pUCSTbl);
			pUCSTbl->close();
		}

		AcDbViewportTable* pViewportTbl;
		stat = pDb->getViewportTable(pViewportTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[ViewportTable]\r\n"));
			ADSKprintSymbolName(pViewportTbl);
			pViewportTbl->close();
		}

		AcDbViewTable* pViewTbl;
		stat = pDb->getViewTable(pViewTbl, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[ViewTable]\r\n"));
			ADSKprintSymbolName(pViewTbl);
			pViewTbl->close();
		}

		AcDbDictionary* pDict;
		stat = pDb->getNamedObjectsDictionary(pDict, kForRead);
		if (stat == eOk) {
			acutPrintf(_T("[Named Object Dictionary]\r\n"));
			const auto pItr = pDict->newIterator();
			AcDbObject* pObj;
			for (; !pItr->done(); pItr->next()) {				
				TCHAR* pszName;
				if (stat == eOk) {
					acutPrintf(_T("  %s\r\n"), pItr->name());
				}
			}

			delete pItr;
			pDict->close();
		}
	}	

#pragma endregion
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(ARXTrainingApp)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, MyCommand, MyCommand, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX2, EX2, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX3, EX3, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX4, EX4, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX5, EX5, ACRX_CMD_MODAL, NULL)
