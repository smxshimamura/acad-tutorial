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
#include "common.h"
#include "ADSKMyEntity.h"
#include "ADSKMyCircle.h"
#include "ADSKMyDictionary.h"
#include "CMyDlg.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

using namespace std;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class ARXTrainingApp : public AcRxArxApp {

public:
	ARXTrainingApp() { }
	virtual ~ARXTrainingApp() = default;

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
		AcDbCircle* pCircle;
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
			for (; !pItr->done(); pItr->next()) {				
				if (stat == eOk) {
					acutPrintf(_T("  %s\r\n"), pItr->name());
				}
			}

			delete pItr;
			pDict->close();
		}
	}	

#pragma endregion

#pragma region Lesson6

	static void ADSKMyGroupEX6() {
		CAcModuleResourceOverride resOverride;
		const auto pDlg = new CMyDlg(acedGetAcadFrame());
		pDlg->Create(IDD_DIALOG1);
		pDlg->ShowWindow(SW_SHOW);
	}

#pragma endregion

#pragma region Lesson7

	static void ADSKMyGroupEX7a() {
		DocVars.docData().attachDatabase(acdbHostApplicationServices()->workingDatabase());
	}

	static void ADSKMyGroupEX7b() {
		DocVars.docData().detachDatabase();
	}
#pragma endregion

#pragma region Lesson8

	static void ADSKMyGroupEX8() {
		ads_point ptVex1;
		auto res = acedGetPoint(nullptr, _T("Set vertex1:"), ptVex1);
		if (res != RTNORM) return;

		ads_point ptVex2;
		res = acedGetPoint(ptVex1, _T("\r\nSet vertex2:"), ptVex2);
		if (res != RTNORM) return;

		ads_point ptVex3;
		res = acedGetPoint(ptVex2, _T("\r\nSet vertex3:"), ptVex3);
		if (res != RTNORM) return;

		ADSKMyEntity* pEnt;
		AcDbBlockTable* pTbl = nullptr;
		AcDbBlockTableRecord* pRec = nullptr;
		{
			pEnt = new ADSKMyEntity();
			pEnt->setVertex1(AcGePoint3d(ptVex1[X], ptVex1[Y], ptVex1[Z]));
			pEnt->setVertex2(AcGePoint3d(ptVex2[X], ptVex2[Y], ptVex2[Z]));
			pEnt->setVertex3(AcGePoint3d(ptVex3[X], ptVex3[Y], ptVex3[Z]));

			const auto pDb = acdbHostApplicationServices()->workingDatabase();
			auto stat = pDb->getBlockTable(pTbl, kForRead);
			if (stat != eOk) goto finally;

			stat = pTbl->getAt(MODEL_SPACE, pRec, kForWrite);
			if (stat != eOk) goto finally;

			stat = pRec->appendAcDbEntity(pEnt);
			if (stat != eOk) goto finally;
		}
		finally:
		if (pRec != nullptr) pRec->close();
		if (pTbl != nullptr) pTbl->close();
		if (pEnt != nullptr) pEnt->close();
	}

#pragma endregion

#pragma region Lesson9

	static void ADSKMyGroupEX9() {
		const auto pDb = new AcDbDatabase(false);
		{
			const auto pFile = _T("C:\\tmp\\test.dwg");
			AcDbBlockTable* pTbl = nullptr;
			AcDbBlockTableRecord* pRec = nullptr;
			AcDbLine* pLine = nullptr;
			{
				auto stat = pDb->readDwgFile(pFile);
				if (stat != eOk) goto finally;

				stat = pDb->getBlockTable(pTbl, kForRead);
				if (stat != eOk) goto finally;

				stat = pTbl->getAt(MODEL_SPACE, pRec, kForWrite);
				if (stat != eOk) goto finally;

				const AcGePoint3d ptStat(0.0, 0.0, 0.0);
				const AcGePoint3d ptEnd(500.0, 500.0, 1.0);
				pLine = new AcDbLine(ptStat, ptEnd);
				stat = pRec->appendAcDbEntity(pLine);
				if (stat != eOk) goto finally;

			}
			finally:
			if (pLine != nullptr) pLine->close();
			if (pRec != nullptr) pRec->close();
			if (pTbl != nullptr) pTbl->close();

			pDb->saveAs(pFile);
		}
		delete pDb;
	}

#pragma endregion

#pragma region Lesson10

	static void ADSKMyGroupEX10() {
		ads_name sSet;
		auto res = acedSSGet(nullptr, nullptr, nullptr, nullptr, sSet);
		if (res != RTNORM) return;

		int nTotal;
		res = acedSSLength(sSet, &nTotal);
		if (res != RTNORM) return;

		AcDbObjectId objId;
		AcDbObjectIdArray objIdAry;
		int stat;
		for (long nIndex = 0; nIndex < nTotal; nIndex++) {
			ads_name name;
			res = acedSSName(sSet, nIndex, name);
			if (res != RTNORM) continue;
			
			stat = acdbGetObjectId(objId, name);
			if (stat != eOk) continue;

			objIdAry.append(objId);
		}

		const auto pExtDb = new AcDbDatabase(false);
		{
			const auto pFile = _T("C:\\tmp\\test.dwg");
			stat = pExtDb->readDwgFile(pFile);
			if (stat != eOk) goto finally;

			const auto pCurDb = acdbHostApplicationServices()->workingDatabase();
			AcDbDatabase* pTmpDb;
			{
				stat = pCurDb->wblock(pTmpDb, objIdAry, AcGePoint3d::kOrigin);
				if (stat != eOk) goto finally;

				stat = pExtDb->insert(AcGeMatrix3d::kIdentity, pTmpDb);
				if (stat != eOk) goto finally;

				pExtDb->saveAs(pFile);
			}
			finally:
			delete pTmpDb;
		}
		delete pExtDb;
	}

#pragma endregion

#pragma region Lesson11

	static void ADSKMyGroupEX11() {
		ads_point ptCenter;
		auto res = acedGetPoint(nullptr, _T("\r\nSet Center:"), ptCenter);
		if (res != RTNORM) return;

		double dRadius;
		res = acedGetDist(ptCenter, _T("\r\nSet radius:"), &dRadius);
		if (res != RTNORM) return;

		const AcGePoint3d pt(ptCenter[X], ptCenter[Y], ptCenter[Z]);
		const auto pCircle = new ADSKMyCircle();
		pCircle->setCenter(pt);
		pCircle->setRadius(dRadius);
		pCircle->setColorIndex(1);

		const auto pDb = acdbHostApplicationServices()->workingDatabase();

		AcDbBlockTable* pTbl = nullptr;
		AcDbBlockTableRecord* pRec = nullptr;
		{
			auto stat = pDb->getBlockTable(pTbl, kForRead);
			if (stat != eOk) goto finally;

			stat = pTbl->getAt(MODEL_SPACE, pRec, kForWrite);
			if (stat != eOk) goto finally;

			pRec->appendAcDbEntity(pCircle);
		}
		finally:
		if (pRec != nullptr) pRec->close();
		if (pTbl != nullptr) pTbl->close();

		pCircle->close();
	}

#pragma endregion

#pragma region Lesson12

	static void ADSKMyGroupEX12() {
		ads_name ename;
		ads_point ptRes;
		auto res = acedEntSel(_T("\r\nSelect object:"), ename, ptRes);
		if (res != RTNORM) return;

		AcDbObjectId objId;
		auto stat = acdbGetObjectId(objId, ename);
		if (stat != eOk) return;

		AcDbEntity* pEnt = nullptr;
		AcDbDictionary* pNamedDict = nullptr;
		AcDbDictionary* pMyDict = nullptr;
		ADSKMyDictionary* pMyDictItem = nullptr;
		stat = acdbOpenAcDbEntity(pEnt, objId, kForRead);
		if (stat != eOk) return;
		{
			if (ADSKMyCircle::desc() != pEnt->isA()) {
				acutPrintf(_T("\r\nInvalid entity. Please select MyCircle entity."));
				goto finally;
			}

			auto nColor = pEnt->colorIndex();
			AcDbHandle hHandle;
			pEnt->getAcDbHandle(hHandle);
			TCHAR szHandle[17];
			if (!hHandle.getIntoAsciiBuffer(szHandle)) {
				if (stat != eOk) goto finally;
			}

			const auto pDb = acdbHostApplicationServices()->workingDatabase();
			stat = pDb->getNamedObjectsDictionary(pNamedDict, kForWrite);
			if (stat != eOk) goto finally;

			const auto dictName = _T("MYDICTIONARY");
			if (pNamedDict->has(dictName) == kFalse) {
				pMyDict = new AcDbDictionary();
				AcDbObjectId dictId;
				stat = pNamedDict->setAt(dictName, pMyDict, dictId);
				if (stat != eOk) goto finally;
				acutPrintf(_T("Added MYDICTIONARY to dictionary."));
			}
			else {
				stat = pNamedDict->getAt(dictName, pMyDict, kForWrite);
				if (stat != eOk) goto finally;
				acutPrintf(_T("MYDICTIONARY is already added."));
			}

			if (pMyDict->has(szHandle) == kFalse) {
				pMyDictItem = new ADSKMyDictionary();
				stat = pMyDictItem->setColor(nColor);
				if (stat != eOk) goto finally;

				AcDbObjectId dictId;
				stat = pMyDict->setAt(szHandle, pMyDictItem, dictId);
				if (stat != eOk) goto finally;

				acutPrintf(_T("Added entry[%s]."), szHandle);
			}
			else {
				acutPrintf(_T("Entry[%s] is already added."), szHandle);
			}
		}
		finally:
		if (pMyDictItem != nullptr) pMyDictItem->close();
		if (pNamedDict != nullptr) pNamedDict->close();
		if (pMyDict != nullptr) pMyDict->close();
		if (pEnt != nullptr) pEnt->close();
	}

#pragma endregion

#pragma region Lesson13

	static void ADSKMyGroupEX13() {
		const auto pDb = acdbHostApplicationServices()->workingDatabase();
		AcDbDictionary* pNamedDict = nullptr;
		AcDbDictionary* pMyDict = nullptr;
		AcDbDictionaryIterator* pItr = nullptr;
		auto stat = pDb->getNamedObjectsDictionary(pNamedDict, kForRead);
		if (stat != eOk) goto finally;;

		const auto dictName = _T("MYDICTIONARY");
		if (pNamedDict->has(dictName) == kFalse) {
			acutPrintf(_T("\r\nDictionary[%s] is not found."), dictName);
		}
		else {
			stat = pNamedDict->getAt(dictName, pMyDict, kForRead);
			if (stat != eOk) goto finally;;

			Adesk::UInt16 nColor;
			AcDbSoftPointerId objId;
			ADSKMyDictionary* pMyDictItem;
			pItr = pMyDict->newIterator();
			for (; !pItr->done(); pItr->next()) {
				stat = pItr->getObject(pMyDictItem, kForRead);
				if (stat == eOk) {
					pMyDictItem->color(nColor);
					pMyDictItem->referObjId(objId);

					acutPrintf(_T("\r\n%s: %d = %d"), pItr->name(), nColor, objId);

					pMyDictItem->close();
				}
			}
		}
		finally:
		if (pItr != nullptr) delete pItr;
		if (pMyDict != nullptr) pMyDict->close();
		if (pNamedDict != nullptr) pNamedDict->close();
	}

#pragma endregion

#pragma region Lesson13

	static void ADSKMyGroupEX14() {
		ads_name entRes;
		ads_point ptRes;
		auto res = acedEntSel(_T("Select the linked object:"), entRes, ptRes);
		if (res != RTNORM) return;

		AcDbObjectId objId;
		auto stat = acdbGetObjectId(objId, entRes);
		if (stat != eOk) return;

		AcDbEntity* pEnt = nullptr;
		AcDbDictionary* pNamedDict = nullptr;
		AcDbDictionary* pMyDict = nullptr;
		ADSKMyDictionary* pMyDictItem = nullptr;
		stat = acdbOpenAcDbEntity(pEnt, objId, kForRead);
		if (stat != eOk) return;
		{
			if (ADSKMyCircle::desc() != pEnt->isA()) {
				acutPrintf(_T("\r\nInvalid entity. Please select MyCircle entity."));
				goto finally;
			}

			auto nColor = pEnt->colorIndex();
			AcDbHandle hHandle;
			pEnt->getAcDbHandle(hHandle);
			TCHAR szHandle[17];
			if (!hHandle.getIntoAsciiBuffer(szHandle)) {
				if (stat != eOk) goto finally;
			}

			const auto pDb = acdbHostApplicationServices()->workingDatabase();
			stat = pDb->getNamedObjectsDictionary(pNamedDict, kForWrite);
			if (stat != eOk) goto finally;

			const auto dictName = _T("MYDICTIONARY");
			if (pNamedDict->has(dictName) == kFalse) {
				pMyDict = new AcDbDictionary();
				AcDbObjectId dictId;
				stat = pNamedDict->setAt(dictName, pMyDict, dictId);
				if (stat != eOk) goto finally;
				acutPrintf(_T("Added MYDICTIONARY to dictionary."));
			}
			else {
				stat = pNamedDict->getAt(dictName, pMyDict, kForWrite);
				if (stat != eOk) goto finally;
				acutPrintf(_T("MYDICTIONARY is already added."));
			}

			AcDbObjectId dictId;
			if (pMyDict->has(szHandle) == kFalse) {
				pMyDictItem = new ADSKMyDictionary();
				stat = pMyDictItem->setColor(nColor);
				if (stat != eOk) goto finally;
				stat = pMyDictItem->setReferObjId(objId);
				if (stat != eOk) goto finally;
				
				stat = pMyDict->setAt(szHandle, pMyDictItem, dictId);
				if (stat != eOk) goto finally;

				acutPrintf(_T("Added entry[%s]."), szHandle);
			}
			else {
				acutPrintf(_T("Entry[%s] is already added."), szHandle);
			}

			res = acedEntSel(_T("Select the referenced object:"), entRes, ptRes);
			if (res != RTNORM) goto finally;

			stat = acdbGetObjectId(objId, entRes);
			if (stat != eOk) goto finally;

			pEnt->close();
			stat = acdbOpenAcDbEntity(pEnt, objId, kForWrite);
			if (stat != eOk) goto finally;

			if (ADSKMyCircle::desc() == pEnt->isA()) {
				acutPrintf(_T("The MyCircle can not be selected."));
				goto finally;
			}

			pEnt->addPersistentReactor(dictId);
		}
		finally:
		if (pMyDictItem != nullptr) pMyDictItem->close();
		if (pNamedDict != nullptr) pNamedDict->close();
		if (pMyDict != nullptr) pMyDict->close();
		if (pEnt != nullptr) pEnt->close();
	}

#pragma endregion
} ;

IMPLEMENT_ARX_ENTRYPOINT(ARXTrainingApp)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, MyCommand, MyCommand, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX2, EX2, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX3, EX3, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX4, EX4, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX5, EX5, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX6, EX6, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX7a, EX7a, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX7b, EX7b, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX8, EX8, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX9, EX9, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX10, EX10, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX11, EX11, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX12, EX12, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX13, EX13, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(ARXTrainingApp, ADSKMyGroup, EX14, EX14, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
