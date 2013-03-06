#include <v8.h>
#include <node.h>

#include "src/odpconnection.h"
#include "src/odpcommand.h"
#include "src/odpdata.h"

using namespace v8;

void initialize(Handle<Object> target){
	OdpConnection::Init(target);
	OdpCommand::Init(target);
	OdpData::Init(target);
}

NODE_MODULE(odpnode, initialize);