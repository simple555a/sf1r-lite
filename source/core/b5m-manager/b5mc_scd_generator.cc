#include "b5mc_scd_generator.h"
#include "b5m_types.h"
#include "b5m_helper.h"
#include <common/ScdParser.h>
#include <common/ScdWriter.h>
#include <common/ScdMerger.h>
#include <common/Utilities.h>
#include <product-manager/product_price.h>
#include <product-manager/uuid_generator.h>
#include <am/sequence_file/ssfr.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

using namespace sf1r;


B5mcScdGenerator::B5mcScdGenerator(OfferDb* odb)
:odb_(odb)
{
}

void B5mcScdGenerator::Process_(Document& doc, int& type)
{
    static const std::string oid_property_name = "ProdDocid";
    if(type==DELETE_SCD)
    {
        type = NOT_SCD;
    }
    else
    {
        type = INSERT_SCD;
        std::string soid;
        if(!doc.getString(oid_property_name, soid))
        {
            type = NOT_SCD;
            return;
        }
        //std::string spid = sdocid;
        std::string spid;
        if(odb_->get(soid, spid))
        {
            
        }
        //keep the non-exist oid comment
        doc.property("uuid") = UString(spid, UString::UTF_8);
    }
}

bool B5mcScdGenerator::Generate(const std::string& scd_path, const std::string& mdb_instance)
{
    if(!odb_->is_open())
    {
        if(!odb_->open())
        {
            LOG(ERROR)<<"odb open fail"<<std::endl;
            return false;
        }
    }
    ScdMerger::PropertyConfig config;
    config.output_dir = B5MHelper::GetB5mcPath(mdb_instance);
    B5MHelper::PrepareEmptyDir(config.output_dir);
    config.property_name = "DOCID";
    config.merge_function = &ScdMerger::DefaultMergeFunction;
    config.output_function = boost::bind(&B5mcScdGenerator::Process_, this, _1, _2);
    config.output_if_no_position = true;
    ScdMerger merger;
    merger.AddPropertyConfig(config);
    merger.AddInput(scd_path);
    merger.Output();
    return true;
}
