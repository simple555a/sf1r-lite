#ifndef SF1R_PRODUCTMANAGER_PRODUCTMANAGER_H
#define SF1R_PRODUCTMANAGER_PRODUCTMANAGER_H

#include <common/type_defs.h>

#include <document-manager/Document.h>
#include <ir/index_manager/index/IndexerDocument.h>

#include "pm_def.h"
#include "pm_types.h"
#include "pm_config.h"
#include "product_price.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace sf1r
{

class ProductDataSource;
class OperationProcessor;
class ProductBackup;
class ProductPriceTrend;

class ProductManager
{
public:
    ProductManager(
            const std::string& collection_name,
            ProductDataSource* data_source,
            OperationProcessor* op_processor,
            const PMConfig& config);

    ~ProductManager();

    bool Recover();

    bool HookInsert(PMDocumentType& doc, izenelib::ir::indexmanager::IndexerDocument& index_document, time_t timestamp);

    bool HookUpdate(PMDocumentType& to, izenelib::ir::indexmanager::IndexerDocument& index_document, time_t timestamp, bool r_type);

    bool HookDelete(uint32_t docid, time_t timestamp);

    bool Finish();

    //update documents in A, so need transfer
    bool UpdateADoc(const Document& doc, bool backup = true);

    //all intervention functions.
    bool AddGroup(const std::vector<uint32_t>& docid_list, izenelib::util::UString& gen_uuid, bool backup = true);

    bool AppendToGroup(const izenelib::util::UString& uuid, const std::vector<uint32_t>& docid_list, bool backup = true);

    bool RemoveFromGroup(const izenelib::util::UString& uuid, const std::vector<uint32_t>& docid_list, bool backup = true);

    bool AddGroupWithInfo(const std::vector<uint32_t>& docid_list, const Document& doc, bool backup = true);

    bool AddGroupWithInfo(const std::vector<izenelib::util::UString>& docid_list, const Document& doc, bool backup = true);

    bool GetPrice(uint32_t docid, ProductPrice& price) const;

    bool GetPrice(const PMDocumentType& doc, ProductPrice& price) const;

    void GetPrice(const std::vector<uint32_t>& docid_list, ProductPrice& price) const;

    bool GetUuid(const PMDocumentType& doc, izenelib::util::UString& uuid) const;

    bool GetDOCID(const PMDocumentType& doc, izenelib::util::UString& docid) const;

    bool GetTimestamp(const PMDocumentType& doc, time_t& timestamp) const;

    bool GetMultiPriceHistory(
            PriceHistoryList& history_list,
            const std::vector<std::string>& docid_list,
            time_t from_tt,
            time_t to_tt);

    bool GetMultiPriceRange(
            PriceRangeList& range_list,
            const std::vector<std::string>& docid_list,
            time_t from_tt,
            time_t to_tt);

    inline const std::string& GetLastError() const
    {
        return error_;
    }

    inline const PMConfig& GetConfig() const
    {
        return config_;
    }

private:
    bool GenOperations_();

    void BackupPCItem_(const izenelib::util::UString& uuid, const std::vector<uint32_t>& docid_list, int type);

    bool UpdateADoc_(const Document& doc);

    bool AppendToGroup_(const izenelib::util::UString& uuid, const std::vector<uint32_t>& uuid_docid_list, const std::vector<uint32_t>& docid_list, const PMDocumentType& uuid_doc);

    void SetItemCount_(PMDocumentType& doc, uint32_t item_count);

private:
    ProductPriceTrend* price_trend_;
    ProductDataSource* data_source_;
    OperationProcessor* op_processor_;
    ProductBackup* backup_;
    PMConfig config_;
    std::string error_;
    bool inhook_;
    boost::mutex human_mutex_;
};

}

#endif
