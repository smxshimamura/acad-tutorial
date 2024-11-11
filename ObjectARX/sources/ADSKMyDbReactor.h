// ReSharper disable CppInconsistentNaming
#pragma once

#include "dbmain.h"

class ADSKMyDbReactor final : public AcDbDatabaseReactor {

public:
	ACRX_DECLARE_MEMBERS(ADSKMyDbReactor);

public:
	ADSKMyDbReactor(AcDbDatabase* pDb = nullptr);
	~ADSKMyDbReactor() override;

	void Attach(AcDbDatabase* pDb);
	void Detach();
	AcDbDatabase* Subject() const;
	bool IsAttached() const;

	void objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj) override;
	void objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased) override;

private:
	AcDbDatabase* mpDatabase;
};

ACDB_REGISTER_OBJECT_ENTRY_AUTO(ADSKMyDbReactor)
