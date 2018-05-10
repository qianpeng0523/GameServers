#include "ccEvent.h"
#include "EventDispatcher.h"
#include "LogicServerInfo.h"
#include "Common.h"
ccEvent::ccEvent(int pcmd, const char* data, int size, int fd, string servername) :
msg(NULL){
	m_cmd = pcmd;
	m_fd = fd;
	m_servername = servername;
	m_type = LogicServerInfo::getIns()->getServerType(servername);
	parse(data, size);
}

void ccEvent::parse(const char* data, int size){

	std::string type_name = EventDispatcher::getIns()->getProtoName(m_cmd, m_type);
	if (type_name.empty()){
		m_type = NO_TYPE;
		type_name = EventDispatcher::getIns()->getProtoName(m_cmd, m_type);
	}
	if (!type_name.empty()){
		msg = create_message(type_name);
		msg->ParseFromArray(data, size);

		string ss = msg->DebugString();
		CLog::log("ccEvent[0x%04X]:(%s)%s\n",m_cmd,type_name.c_str(),ss.c_str());
	}
	delete data;
}

::google::protobuf::Message* ccEvent::create_message(const std::string& type_name)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor = \
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor)
	{
		const google::protobuf::Message* prototype =
			::google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}
