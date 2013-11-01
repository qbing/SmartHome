/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      CDeviceConnection.cpp
 * Description:   CDeviceConnection class
 * Author:        QBryan	2013/09/10 23:21:00
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.QBryan 2013/09/10 23:21:00->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include "SmartHomeInfr.h"

CDeviceConnection::CDeviceConnection(void)
{
	treeNode = NULL;
	timeout_second = 0;
}

CDeviceConnection::~CDeviceConnection(void)
{
	if(treeNode){
		treeNode->device_conn=NULL;
		treeNode->setIsActive(false);
		/*//////////update to file////////begin///////////////////*/{
		sys.shp_update_system_device_tree_infr_to_file();
		/*//////////update to file////////end/////////////////////*/}
	}
}

/******************************************************************************
 * FUNCTION NAME:	shp_handle_recviced_data
 * DESCRIPTION:	    This function handle the recived data from Networt or UART 
					or SPI or Bluetooth or somewhere else!!!!!
					You MUST call this fuction when data arrived!!!!!!!
 * modification history
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int CDeviceConnection::handle_recieved_data(U8* bin,U16 len)
{
	int used = 0;
	buf.PutData(bin,len);
	
	/*/////handle///////begin///*/{
	do{
		DBG("There are %d Bytes Data want to handle!!!!\r\n",buf.GetLengh());
		used = handle_recieved_buffer(buf);
		DBG("handle_recieved_buffer Used %d Bytes Data\r\n",used);
		if(used>0){
			buf.SetUsed(used);
		}else if(used<0){
			buf.SetUsed(buf.GetLengh());
		}
	}while(used>0 && buf.GetLengh()>0);
	/*/////handle///////end/////*/}
	return len;
}

/******************************************************************************
 * FUNCTION NAME:	handle_recieved_buffer
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int CDeviceConnection::handle_recieved_buffer(CDataBuffer& buf)
{
	SHP_HeadForRecv* head = (SHP_HeadForRecv*)buf.GetData();
	DBG("handle_recieved_buffer...\r\n");
	//shp_dbg_mem((char*)head,32);
	if(head->magic1 == SHP_MAGIC1){
		if(head->pkg_len <= buf.GetLengh() && head->pkg_len >0){
			if(head->magic2 == SHP_MAGIC2)
			{
				int used = 0;
				DBG("handleing...\r\n");
				switch(head->content_type)
				{
				case 'B':
					//handle
					used = shp_handle_binary_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				case 'J':
					//handle
					used = shp_handle_json_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				case 'X':
					//handle
					used = shp_handle_xml_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				case 'C':
					//Consultation
					used = shp_handle_consult_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				default:
					//Unsurported content type
					DBG("Unsurported content type 0X%02X\r\n",head->content_type);
					return head->pkg_len;
				}
				DBG("handle success!!!!!!!!!!!\r\n");
				/////////////used size////////begin///////
				switch(used)
				{
				case SHP_USED_SIZE_ALL:
					return head->pkg_len;
				case SHP_USED_SIZE_ERROR:
					return SHP_USED_SIZE_ERROR;
				default:
					return used>0 ? used +sizeof(SHP_HeadForRecv):-1;
				}
				/////////////used size////////end/////////
			}else{
				//MAGIC2 is not right!!!!
				DBG("MAGIC2 is not right!!!!\r\n");
				return head->pkg_len;
			}
		}else{
			DBG("too few data to handle!!\r\n");
			return 0;
		}
	}else if(buf.GetLengh() >= sizeof(SHP_HeadForRecv)){
		DBG("MAGIC1 is not right or Data error!!! Skip one byte!!\r\n");
		return 1;//判定为出错，跳过一个字节
	}else{
		DBG("MAGIC1 is not right!!!!\r\n");
		return 0;
	}
}

/******************************************************************************
 * FUNCTION NAME:	CDeviceConnection.shp_handle_consult_data
 * DESCRIPTION:		Handle the consult data,
 					Devices *MUST* support this PROTOCOL!!!!
 ******************************************************************************/
int  CDeviceConnection::shp_handle_consult_data(void* bin,int size)
{
	U16 pid = *((U16*)bin);
	switch(pid)
	{
		case SHP_CONSULT_S1_PID:{
			SHP_CONSULT_PKG pkg;
			state = SHP_SVR_STATE_CONSULTATION_S1;
			pkg.pid = pid;
			memcpy(mac,((U8*)bin)+sizeof(pid),sizeof(mac));
			memcpy(pkg.mac,mac,sizeof(mac));
			memcpy(pkg.surport_or_not,"S0",2);
			shp_send_package('C',&pkg,sizeof(pkg));
			}break;
		case SHP_CONSULT_S3_PID:{
			SHP_CONSULT_PKG pkg;
			pkg.pid = pid;
			memcpy(pkg.mac,mac,sizeof(mac));
			if(SHP_SVR_STATE_CONSULTATION_S1==state){
				/*////save device infromation//////////begin//*/{
				shp_update_system_device_tree_infr(((const char*)bin)+sizeof(pid),size-sizeof(pid));
				/*////save device infromation//////////end////*/}
				state = SHP_SVR_STATE_RUNNING;
				memcpy(pkg.surport_or_not,"S1",2);
			}else{
				state = SHP_SVR_STATE_CONSULTATION_S0;
				memcpy(pkg.surport_or_not,"N0",2);
			}
			shp_send_package('C',&pkg,sizeof(pkg));
			break;
		}
		default:
			break;
	}
	return SHP_USED_SIZE_ALL;
}
int CSystemInfr::shp_update_system_device_tree_infr_to_file(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	#define STATIC_MD5_ID "012345678912345"
	
	#define JSON_TREE_FILLNAME	"./../SmartHomeWeb/SmartHomeView.html"
	extern int my_memcmp(void* mem1,void*mem2,int size);
	
	static char tree_jsonstr_md5[] = STATIC_MD5_ID;
	static char new_json_md5_ret[] = STATIC_MD5_ID;
	std::string sysJsonTree = dev_tree.toJsonString();
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	//get new jsonstr md5
	char* temp_mdt = MyMD5(sysJsonTree.c_str(),sysJsonTree.length());
	if(NULL==temp_mdt)return SHP_ERROR;
	memcpy(new_json_md5_ret,temp_mdt,16);


	//get old jsonstr md5
	if(1)//strcmp(tree_jsonstr_md5,STATIC_MD5_ID)==0)
	{
		char* file_md5_ret = NULL;
		FILE* fp = fopen(JSON_TREE_FILLNAME,"r");
		if(fp!=NULL){
			int flen = 0;
			char * file_buf = NULL;
			fseek(fp,0L,SEEK_END);
			flen=ftell(fp);
			file_buf = new char[flen+2];
			fseek(fp, 0, SEEK_SET);     
			fread(file_buf, flen , 1, fp);
			
			file_md5_ret = MyMD5(file_buf,flen);
			if(file_md5_ret){
				memcpy(tree_jsonstr_md5,file_md5_ret,16);
			}else{
				memset(tree_jsonstr_md5,0,16);
			}
			delete[] file_buf;
			fclose(fp);
		}else{
			memset(tree_jsonstr_md5,0,16);
			DBG_BREIF("open json tree file %s failed!\r\n",JSON_TREE_FILLNAME);
		}
	}

	//if the json str is modified,then write the new jsonstr to file.
	//shp_dbg_mem(new_json_md5_ret,16);
	//shp_dbg_mem(tree_jsonstr_md5,16);
	if(0!=my_memcmp(new_json_md5_ret,tree_jsonstr_md5,16))
	{
		FILE* fp = fopen(JSON_TREE_FILLNAME,"w");
		if(fp!=NULL){
			DBG_BREIF("wirte json tree to file %s ....\r\n",JSON_TREE_FILLNAME);
			fprintf(fp,"%s",sysJsonTree.c_str());
			memcpy(tree_jsonstr_md5,new_json_md5_ret,16);
			fclose(fp);
			DBG_BREIF("wirte json tree to file %s done\r\n",JSON_TREE_FILLNAME);
		}else{
			DBG_BREIF("open json tree file %s failed!\r\n",JSON_TREE_FILLNAME);
		}
	}else{
		//shp_dbg_mem(tree_jsonstr_md5,16);
		DBG_BREIF("json tree is not modified!\r\n",JSON_TREE_FILLNAME);
	}

	DBG_BREIF("shp_update_system_device_tree_infr_to_file...DONE!\r\n");
	return SHP_OK;
}


int CDeviceConnection::shp_update_system_device_tree_infr(const char* json_str,int len)
{
	DBG_BREIF("shp_update_system_device_tree_infr...\r\n");
	sys.dev_tree.shp_parse_device_tree(this,json_str,len);
	DBG_BREIF("shp_update_system_device_tree_infr end!\r\n");
	/*//////////update to file////////begin///////////////////*/{
	sys.shp_update_system_device_tree_infr_to_file();
	/*//////////update to file////////end/////////////////////*/}
	return SHP_USED_SIZE_ALL;
}


CDeviceTree::CDeviceTree()
{
    memset(name,0,sizeof(name));
    device_conn = NULL;
	isActive = true;
    type = TYPE_DEV;
    children_nodes.clear();
	atom = NULL;

}
CDeviceTree::~CDeviceTree()
{
	list<CDeviceTree*>::iterator it = children_nodes.begin();
	list<CDeviceTree*>::iterator end = children_nodes.end();
	while(it!=end){
		delete *it;
		it++;
	}
}

void CDeviceTree::setIsActive(bool active)
{
	list<CDeviceTree*>::iterator it = children_nodes.begin();
	list<CDeviceTree*>::iterator end = children_nodes.end();
	this->isActive = active;
	while(it!=end){
		(*it)->setIsActive(active);
		it++;
	}
}


CDeviceTree* CDeviceTree::findChild_By_name(char* name)
{
	list<CDeviceTree*>::iterator it = children_nodes.begin();
	list<CDeviceTree*>::iterator end = children_nodes.end();
	while(it!=end){
		DBG("Scan tree:%s...\r\n",(*it)->name);
		if(0==strcmp((*it)->name,name)){
			return *it;
		}
		it++;
	}
	return NULL;
}


CDeviceTree* CDeviceTree::shp_parse_device_tree(CDeviceConnection*conn,const char* json_str,int len)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	Json::Reader 	reader; 
	Json::Value		root; 
	CDeviceTree	*	tree = NULL;
	bool parser_ret	 = reader.parse((const char*)json_str,(const char*)json_str+len,root,false);
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if(parser_ret){
		DBG("parse success,handle...!!\r\n");
		tree = shp_parse_device_tree(root);
		
		conn->treeNode = tree;		//one connect map to one tree
		tree->device_conn = conn;	//one connect map to one tree
		
		if(tree){
			list<CDeviceTree*>::iterator it = children_nodes.begin();
			list<CDeviceTree*>::iterator end = children_nodes.end();
			while(it!=end){
				if(memcmp((*it)->mac,tree->mac,6)==0){
					delete *it;
					*it = tree;
					break;
				}
				it++;
			}
			if(it==end)children_nodes.push_back(tree);
		}
	}else{
		DBG("The json string is invalid!!\r\n");
	}
	return 0;
}

CDeviceTree* CDeviceTree::shp_parse_device_tree(Json::Value root)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	Json::Value		temp; 
	Json::Value		child; 
	CDeviceTree	*	tree = NULL;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//name
	temp = root["NAME"];
	if(temp.isNull()){return NULL;}
	tree = new CDeviceTree();
	strcpy(tree->name,temp.asString().c_str()); 

	//type
	temp = root["TYPE"];
	if(temp.type()!=Json::nullValue)
	{
		DBG("CDeviceTree::type=%s\r\n",temp.asString().c_str());
		tree->type = get_node_type_by_name(temp.asString()); 
		if(tree->type == TYPE_TERM){
			DBG("CDeviceTree find a terminator!!!\r\n");
			tree->atom = CAtomBase::parse_jsonstr(root);
			//if the type is TYPE_TERM, we return directly!!!
			if(tree->atom==NULL)DBG("Warning:type is terminator,but atom is **NULL**\r\n");
			return tree;
		}
	}

	//mac
	temp = root["MAC"];
	if(temp.type()!=Json::nullValue)
	{
		char mac_str[32] = {0};
		strcpy(mac_str,temp.asString().c_str()); 
		sscanf(mac_str, "%2x%2x%2x%2x%2x%2x", &tree->mac[0], &tree->mac[1], &tree->mac[2], &tree->mac[3], &tree->mac[4], &tree->mac[5]);
		strcat(tree->name,mac_str); 
		DBG("parse success,handle mac :%02x%02x%02x%02x%02x%02x...!!\r\n",tree->mac[0],tree->mac[1],tree->mac[2],tree->mac[3],tree->mac[4],tree->mac[5]);
	}
	
	//child
	DBG("parse success,handle mac 1...!!\r\n");
	child 			 = root["CHILD"];
	if(child.type() == Json::arrayValue)
	{
		int i = 0;
		int num=child.size();
		
		for(i=0;i<num;i++){
			CDeviceTree*child_tree = NULL;
			DBG("parse success,handle 3...!!\r\n");
			child_tree = shp_parse_device_tree(child[i]);
			DBG("parse success,handle 4...!!\r\n");
			if(child_tree){
				tree->children_nodes.push_back(child_tree);
			}else{
				DBG("Warning : The child_tree==NULL, but that should not happened!\r\n");
			}
		}
	}else{
		DBG("No child!!\r\n");
	}
	return tree;
}


std::string CDeviceTree::toJsonString(void)
{
	std::string str="";

	//JUST FOR TEST// donot show inactive device
	if(this->isActive==false)return str;

	//start
	str+="{";
	//name
	str+="\"NAME\":\"";
	str+=name;
	str+="\"";
		
	//dev_id
	if(this->type == TYPE_TERM ){
		if(this->atom!=NULL){
			char *my_itoa(int val, char *buf, unsigned radix);
			char id_str[64];
			str+=",\"DEV_ID\":";
			str+=my_itoa((int)this->atom->id,NULL,10);
			str+=',';
			str+=this->atom->tostring();
		}else{
			DBG("Warning:type is terminator,but atom is **NULL**\r\n");		
		}
	}

	//children
	if(children_nodes.size()>=1){
		str+=",";
		str+="\"CHILD\":[";
		list<CDeviceTree*>::iterator it = children_nodes.begin();
		list<CDeviceTree*>::iterator end = children_nodes.end();
		while(it!=end){
			str+=(*it)->toJsonString();
			it++;
			if(it!=end)str+=",";
		}
		str+="]";
	}
	str+="}";
	return str;
}

CDeviceTree::DEVICE_NODE_TYPE CDeviceTree::get_node_type_by_name(std::string name)
{
	if(name == "DEV"){
		return TYPE_DEV;
	}else if(name == "SET"){
		return TYPE_SET;
	}else if(name == "TERM"){
		return TYPE_TERM;
	}else{
        //return TYPE_INVALID;
        return TYPE_SET;
	}
}

std::string CDeviceTree::get_name_by_node_type(DEVICE_NODE_TYPE node_type)
{
	switch(node_type){
		case TYPE_DEV:
			return std::string("DEV");
		case TYPE_SET:
			return std::string("SET");
		case TYPE_TERM:
			return std::string("TERM");
		default:
			break;
	}
	return std::string("INVALID");
}

CAtomBase* CDeviceTree::find_atom_by_devid(U16 dev_id)
{
	list<CDeviceTree*>::iterator it = children_nodes.begin();
	list<CDeviceTree*>::iterator end = children_nodes.end();

	if(atom!=NULL && atom->id == dev_id){
		return atom;
	}else{
		CAtomBase* temp_atom = NULL;
		while(it!=end){
			temp_atom = (*it)->find_atom_by_devid(dev_id);
			if(temp_atom!=NULL){
				return temp_atom;
			}
			it++;
		}
		return NULL;
	}
}


typedef struct SHP_DEV_TICK_DEVICE_DATA_T
{
	U16 dev_id;
	U16 offset;
	
	//////////////////
	//DATA////////////
	//////////////////
}SHP_DEV_TICK_DEVICE_DATA_T;

int  CDeviceConnection::shp_handle_binary_data(void* bin,int size)
{
	U16 pid = 0;
	char *p = (char*)bin;
	pid = *((U16*)bin);
	p+=2;//skip pid
	switch(pid){
		case SHP_DEV_TICK_PID:/**device tick data**begin**/{
			U16 num = *((U16*)(p));
			int i = 0;
			SHP_DEV_TICK_DEVICE_DATA_T *dat_pkg = NULL;
			char* pData = NULL;
			CAtomBase* atom = NULL;
			
			p+=6;//skip mac
			p+=2;//skip num

			if(this->treeNode)
			{
				DBG("Handling %d TICK data...\r\n",num);
				dat_pkg = (SHP_DEV_TICK_DEVICE_DATA_T*)p;
				for(i=0;i<num;i++){
					pData = (((char*)dat_pkg)+sizeof(SHP_DEV_TICK_DEVICE_DATA_T));
					atom = this->treeNode->find_atom_by_devid(dat_pkg->dev_id);
					if(atom && dat_pkg->offset > sizeof(SHP_DEV_TICK_DEVICE_DATA_T)){
						atom->parse_tick_data(pData,dat_pkg->offset - sizeof(SHP_DEV_TICK_DEVICE_DATA_T));
					}else{
						DBG("Warning: Can not found device by id %d,or tick data is invalid\r\n",dat_pkg->dev_id);
					}
					dat_pkg = (SHP_DEV_TICK_DEVICE_DATA_T*)(((char*)dat_pkg)+dat_pkg->offset);
				}
				DBG("Handle TICK data **DONE**\r\n");
			}else{
				DBG("Warning: This connection have *NO* treeNode!!!\r\n");
			}
			break;/**device tick data**end**/}
		default:
			DBG("Warning: **UNSURPORT** PID\r\n");
			break;
			
	}
	return SHP_USED_SIZE_ALL;
}

int  CDeviceConnection::shp_handle_json_data(char* json_str,int size)
{
	return SHP_USED_SIZE_ALL;
}

int  CDeviceConnection::shp_handle_xml_data(char* xml_json_str,int size)
{
	return SHP_USED_SIZE_ALL;
}

S16  CDeviceConnection::shp_send_package(S8 type,void* bin,S16 len)
{
	SHP_HeadForSend	head={SHP_MAGIC1,sizeof(SHP_HeadForSend),SHP_MAGIC2,type,0};
	int ret = 0;
	head.pkg_len+=len;
	ret = send(socket_id,&head,sizeof(head),0);
	DBG("CDeviceConnection::shp_send_package send %d bytes data\r\n",ret);
	ret = send(socket_id,bin,len,0);
	DBG("CDeviceConnection::shp_send_package send %d bytes data\r\n",ret);	
	return ret;
}




