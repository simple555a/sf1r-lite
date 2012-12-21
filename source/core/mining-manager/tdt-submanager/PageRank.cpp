#include "PageRank.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <boost/thread/shared_mutex.hpp>
#include <pthread.h>
#include <icma/icma.h>
#include <icma/openccxx.h>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
std::ostream& operator<<(std::ostream &f, const Node &node)
{
  //ReadLock lock(mutex_);
  f.write((const char*)&node.id_, sizeof(node.id_));
  f.write((const char*)&node.outNumber_, sizeof(node.outNumber_));
  size_t len=sizeof(node.name_[0])*(node.name_.size()+1);
  f.write((const char*)&len, sizeof(len));
  f.write((const char*)&node.name_[0], sizeof(node.name_[0])*(node.name_.size()+1) );

  len=node.linkin_index_.size();
  f.write((const char*)&len, sizeof(len));
  vector<int>::const_iterator citr = node.linkin_index_.begin();
  for (; citr !=node.linkin_index_.end(); ++citr)
  {
         int  index=(*citr);
         //cout<<"citr"<<index<<endl;
         f.write((const char*)&index, sizeof(index));
  }  
  return f;
  
}

std::istream& operator>>(std::istream &f, Node &node )
{
  size_t len;
 // WriteLock lock(mutex_);

 
  f.read(( char*)&node.id_, sizeof(node.id_));

  f.read(( char*)&node.outNumber_, sizeof(node.outNumber_));

  //size_t len=sizeof(node.name_);
  f.read(( char*)&len, sizeof(len));

  node.name_.resize(len);
  f.read(( char*)&node.name_[0], len);
  //string namestr(name,len);
  //node.name_=namestr;
  // cout<<node.name_<<endl;
  f.read(( char*)&len, sizeof(len));
  int size=len;
  //cout<<"len"<<len<<endl;
  int index;
 // node.linkin_index_.reserve(size);
  node.linkin_index_.resize(size);
  for(int i=0;i<size;i++)
  {

    // cout<<"i"<<i<<endl;
     f.read(( char*)&index, sizeof(index));
    // cout<<"das"<<endl;
     node.linkin_index_[i]=index;
     //cout<<index<<endl;
    // node.linkin_index_.insert(index);
  }
  return f;
 
}
void simplify(Node &node, cma::OpenCC* opencc)
{
    std::string lowercase_content = node.name_;
    boost::to_lower(lowercase_content);
    std::string simplified_content;
    long ret = opencc->convert(lowercase_content, simplified_content);
    if(-1 == ret) simplified_content = lowercase_content;
    node.name_=simplified_content;
}

    Node::Node(string name, int id):name_(name),id_(id)
    {
       contentRelevancy_=0;;
       advertiRelevancy_=0;;
       outNumber_=0;


    }

    Node::~Node()
    {
       linkin_index_.clear();


      //    
    }
    /*
    void InsertLinkdInNode(Node* node) 
    {
        //如果没有链接
        if (linkin_nodes_.find(node) == linkin_nodes_.end()) 
        {
            linkin_nodes_.insert(node);
        }
        node->InsertLinkOutNode(this);
    }

    void InsertLinkOutNode(Node* node) 
    {
        //如果没有链接
        if (linkout_nodes_.find(node) == linkout_nodes_.end()) 
        {
            linkout_nodes_.insert(node);
        }
    }
    */
   

    void Node::InsertLinkInNode(int i) 
    {
        //如果没有链接top
      
        linkin_index_.push_back(i);
    }
/*
    double Node::GetPageRank()
    {
        return page_rank_;
    }

    void Node::SetPageRank(double pr)
    {
        page_rank_ = pr;
    }

    void Node::SetContentRelevancy(double contentRelevancy)
    {
        contentRelevancy_ = contentRelevancy;
    }
*/
    void Node::SetAdvertiRelevancy(double advertiRelevancy)
    {
        advertiRelevancy_ = log(log(advertiRelevancy+1.0)+1.0);
    }
/*
    double Node::CalcRank(const vector<Node*>& vecNode,const CalText& linkinSub)
    {
        double pr = 0;
        /* 
        vector<int>::const_iterator citr = linkin_index_.begin();
       // cout<<"pr"<<pr<<endl;
        for (; citr != linkin_index_.end(); ++citr)
        {
           // cout<<"Link in"<<(*citr)<<endl;
            Node * node = vecNode[(*citr)];
            //node->PrintNode();
           // if(node->GetPageRank()>0.0)
            pr += node->GetPageRank()/double(node->GetOutBoundNum());
           // cout<<"pr"<<pr<<endl;
        }

      
        vector<int>::const_iterator citr = linkinSub.linkin_.begin();
       // cout<<"pr"<<pr<<endl;
        for (; citr != linkinSub.linkin_.end(); ++citr)
        {
           // cout<<"Link in"<<(*citr)<<endl;
            Node * node = vecNode[(*citr)];
            //node->PrintNode();
            pr += node->GetPageRank()/double(node->GetOutBoundNum());
           // cout<<"pr"<<pr<<endl;
        }
        
        return pr;
    }
*/
    size_t Node::GetOutBoundNum() 
    {
        return  outNumber_;
    }
    size_t Node::GetInBoundNum() 
    {
        return  linkin_index_.size();
    }
/*
    double Node::GetContentRelevancy() 
    {
        return contentRelevancy_;
    }
*/
    double Node::GetAdvertiRelevancy() 
    {
        return advertiRelevancy_;
    }
    string Node::GetName() 
    {
        return name_;
    }
    void Node::InsertLinkOutNode(int i) 
    {
         linkout_index_.push_back(i);
         outNumber_++;
    }
    void Node::PrintNode()
    {

        cout << "Node:" <<name_ <<"advertiRelevancy"<<advertiRelevancy_<<"id"<<id_<<"outNumber_"<<outNumber_<<"inNumber"<< linkin_index_.size()<<endl; 
    }
    void Node::SetId(int id)
    {
        id_=id;
    }
    int Node::GetId()
    {
        return id_;
    }
PageRank::PageRank(vector<Node*>& nodes,set<int>& SubGraph,double alpha,double beta) : alpha_(alpha),beta_(beta), nodes_(nodes),SubGraph_(SubGraph)
{
     cout<<"PageRankBuild"<<endl;
    // q_ must < 1
    
}


PageRank::~PageRank(void)
{
}

void PageRank::InitMap()
{
    set<int>::const_iterator citr = SubGraph_.begin();
    for (; citr != SubGraph_.end(); ++citr)
    {
           // cout<<"Link in"<<(*citr)<<endl;
            Node * node =nodes_[(*citr)];

            int oN=0,iN=0;
            vector<int>::const_iterator sitr = node->linkout_index_.begin();
            vector<int> linkin;

           for (; sitr !=node->linkout_index_.end(); ++sitr)
           {
                if(SubGraph_.find(*sitr)!=SubGraph_.end())
                {oN++;}
           
           }
           
           sitr = node->linkin_index_.begin();
           
           for (; sitr !=node->linkin_index_.end(); ++sitr)
           {
                if(SubGraph_.find(*sitr)!=SubGraph_.end())
                {
                     linkin.push_back(*sitr);
                     iN++;
                }
           
           }

           CalText temp;
           temp.linkin_=linkin;
           temp.pr_=1.0;
           temp.contentRelevancy_=0.0;
           temp.outNumber_=oN;
           linkinMap_.insert(pair<int,CalText>((*citr), temp));
           //node->PrintNode();
           //cout<<"innumber"<<iN<<endl;
    }
}
// 迭代计算n次
void PageRank::CalcAll(int n)
{
  /*
    set<int>::const_iterator citr = SubGraph_.begin();
    for (; citr != SubGraph_.end(); ++citr)
    {
           // cout<<"Link in"<<(*citr)<<endl;
            Node * node =nodes_[(*citr)];
            node->outNumberOrg_=node->outNumber_;
            int oN=0,iN=0;
            vector<int>::const_iterator sitr = node->linkout_index_.begin();
            vector<int> linkin;

           for (; sitr !=node->linkout_index_.end(); ++sitr)
           {
                if(SubGraph_.find(*sitr)!=SubGraph_.end())
                {oN++;}
           
           }
           
           sitr = node->linkin_index_.begin();
           
           for (; sitr !=node->linkin_index_.end(); ++sitr)
           {
                if(SubGraph_.find(*sitr)!=SubGraph_.end())
                {
                     linkin.push_back(*sitr);
                     iN++;
                }
           
           }

           CalText_=oN;
           linkinMap_.insert(pair<int,vector<int> >((*citr), linkin));
           //node->PrintNode();
           //cout<<"innumber"<<iN<<endl;
    }
*/
       /**/
    set<int>::const_iterator citr;
    boost::posix_time::ptime time_now = boost::posix_time::microsec_clock::local_time(); 
    cout<<"subset"<<time_now<<endl;
    for (int i=0; i<n; ++i) 
    {
      //  cout<<"for"<<i<<"time"<<endl;
      /* vector<Node*>::iterator citr = nodes_.begin();
        

       // cout<<"pr"<<pr<<endl;

        for (; citr != nodes_.end(); ++citr)
        {
           // cout<<"Link in"<<(*citr)<<endl;
            Node * node =(*citr);
            //node->PrintNode();
           // cout<<"pr"<<pr<<endl;
           Calc(node);
        }
       */
       
        citr = SubGraph_.begin();
       // cout<<"pr"<<pr<<endl;
        for (; citr != SubGraph_.end(); ++citr)
        {
           // cout<<"Link in"<<(*citr)<<endl;

            //node->PrintNode();
           // cout<<"pr"<<pr<<endl;
            Calc(*citr);
        }
       /**/
       
    }
    /*
     for (; citr != SubGraph_.end(); ++citr)
    {
           // cout<<"Link in"<<(*citr)<<endl;
            Node * node =nodes_[(*citr)];
            node->outNumber_=node->outNumberOrg_;
           
    }
    */
    //linkinMap_.clear();
    time_now = boost::posix_time::microsec_clock::local_time(); 
    cout<<"done"<<time_now<<endl;
}

void PageRank::PrintPageRank(vector<Node*> & nodes)
{
    double total_pr = 0;
    vector<Node*>::const_iterator citr = nodes.begin();
    for (; citr!=nodes.end(); ++citr) 
    {    
        Node * node = *citr;
        node->PrintNode();
        //total_pr += node->GetPageRank();
    }
    //cout << "Total PR:" << total_pr << endl;
}

double PageRank::Calc(int index)
{
   // cout<<"CalcRank(nodes_)"<<endl;
    //cout<<"nodes size()"<<nodes_.size()<<endl;
    Node * node =nodes_[index];
    CalText &temp=getLinkin(index);
    double pr = CalcRank(temp);
   // cout<<"pr"<<pr<<endl;
    if (pr < 0.00000000000000000000001 && pr > -0.00000000000000000000001) //pr == 0
    {
        pr = alpha_*temp.contentRelevancy_+beta_*node->GetAdvertiRelevancy() ;
    }
    else 
    {// (1-alpha_-beta_)
        pr = pr*(1-alpha_-beta_)+ alpha_*temp.contentRelevancy_+beta_*node->GetAdvertiRelevancy();
    }
   // cout<<"setPr"<<endl;
   // node->SetPageRank(pr);
    temp.pr_=pr;
    return pr;
}

CalText& PageRank::getLinkin(int index)
{
            map<int,CalText >::iterator it;
            it = linkinMap_.find(index);

            if(it !=linkinMap_.end())
            {

                 return (it->second);
 
            }
            else
            {
                 cout<<"error getLinkin"<<endl;
            }
            
}

double PageRank::getPr(int index)
{
        //cout<<"getPr"<<index<<endl;
        return getLinkin(index).pr_;
            
}
void PageRank::setPr(int index,double pr)
{
      // cout<<"SetPr"<<index<<endl;
       getLinkin(index).pr_=pr;
}
double PageRank::setContentRelevancy(int index,double contentRelevancy)
{
      // cout<<"setContentRelevancy"<<index<<endl;
       getLinkin(index).contentRelevancy_=contentRelevancy;
}
double PageRank::getContentRelevancy(int index)
{
      return   getLinkin(index).contentRelevancy_;
}
double PageRank::getON(int index)
{
       // cout<<"getON"<<index<<endl;
          
        return double(getLinkin(index).outNumber_);
            
}

double PageRank::CalcRank(const CalText& linkinSub)
{
        double pr = 0;
    
        vector<int>::const_iterator citr = linkinSub.linkin_.begin();
       // cout<<"pr"<<pr<<endl;
        for (; citr != linkinSub.linkin_.end(); ++citr)
        {
           // cout<<"Link in"<<(*citr)<<endl;
           // Node * node = nodes_[(*citr)];
            //node->PrintNode();
            pr += getPr(*citr)/getON(*citr);
           // cout<<"pr"<<pr<<endl;
        }
        
        return pr;
}
