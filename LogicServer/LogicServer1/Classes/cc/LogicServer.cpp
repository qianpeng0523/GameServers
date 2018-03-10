// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: LogicServer.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "LogicServer.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace protocol {

namespace {

const ::google::protobuf::Descriptor* CLogicLogin_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CLogicLogin_reflection_ = NULL;
const ::google::protobuf::Descriptor* SLogicLogin_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SLogicLogin_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_LogicServer_2eproto() {
  protobuf_AddDesc_LogicServer_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "LogicServer.proto");
  GOOGLE_CHECK(file != NULL);
  CLogicLogin_descriptor_ = file->message_type(0);
  static const int CLogicLogin_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CLogicLogin, cmd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CLogicLogin, servername_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CLogicLogin, seession_),
  };
  CLogicLogin_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CLogicLogin_descriptor_,
      CLogicLogin::default_instance_,
      CLogicLogin_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CLogicLogin, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CLogicLogin, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CLogicLogin));
  SLogicLogin_descriptor_ = file->message_type(1);
  static const int SLogicLogin_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SLogicLogin, cmd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SLogicLogin, err_),
  };
  SLogicLogin_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SLogicLogin_descriptor_,
      SLogicLogin::default_instance_,
      SLogicLogin_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SLogicLogin, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SLogicLogin, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SLogicLogin));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_LogicServer_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CLogicLogin_descriptor_, &CLogicLogin::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SLogicLogin_descriptor_, &SLogicLogin::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_LogicServer_2eproto() {
  delete CLogicLogin::default_instance_;
  delete CLogicLogin_reflection_;
  delete SLogicLogin::default_instance_;
  delete SLogicLogin_reflection_;
}

void protobuf_AddDesc_LogicServer_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021LogicServer.proto\022\010protocol\"F\n\013CLogicL"
    "ogin\022\021\n\003cmd\030\001 \002(\r:\0044096\022\022\n\nservername\030\002 "
    "\002(\t\022\020\n\010seession\030\003 \001(\t\"-\n\013SLogicLogin\022\021\n\003"
    "cmd\030\001 \002(\r:\0044096\022\013\n\003err\030\002 \001(\r", 148);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "LogicServer.proto", &protobuf_RegisterTypes);
  CLogicLogin::default_instance_ = new CLogicLogin();
  SLogicLogin::default_instance_ = new SLogicLogin();
  CLogicLogin::default_instance_->InitAsDefaultInstance();
  SLogicLogin::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_LogicServer_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_LogicServer_2eproto {
  StaticDescriptorInitializer_LogicServer_2eproto() {
    protobuf_AddDesc_LogicServer_2eproto();
  }
} static_descriptor_initializer_LogicServer_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int CLogicLogin::kCmdFieldNumber;
const int CLogicLogin::kServernameFieldNumber;
const int CLogicLogin::kSeessionFieldNumber;
#endif  // !_MSC_VER

CLogicLogin::CLogicLogin()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CLogicLogin::InitAsDefaultInstance() {
}

CLogicLogin::CLogicLogin(const CLogicLogin& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CLogicLogin::SharedCtor() {
  _cached_size_ = 0;
  cmd_ = 4096u;
  servername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  seession_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CLogicLogin::~CLogicLogin() {
  SharedDtor();
}

void CLogicLogin::SharedDtor() {
  if (servername_ != &::google::protobuf::internal::kEmptyString) {
    delete servername_;
  }
  if (seession_ != &::google::protobuf::internal::kEmptyString) {
    delete seession_;
  }
  if (this != default_instance_) {
  }
}

void CLogicLogin::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CLogicLogin::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CLogicLogin_descriptor_;
}

const CLogicLogin& CLogicLogin::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_LogicServer_2eproto();
  return *default_instance_;
}

CLogicLogin* CLogicLogin::default_instance_ = NULL;

CLogicLogin* CLogicLogin::New() const {
  return new CLogicLogin;
}

void CLogicLogin::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    cmd_ = 4096u;
    if (has_servername()) {
      if (servername_ != &::google::protobuf::internal::kEmptyString) {
        servername_->clear();
      }
    }
    if (has_seession()) {
      if (seession_ != &::google::protobuf::internal::kEmptyString) {
        seession_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CLogicLogin::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 cmd = 1 [default = 4096];
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_servername;
        break;
      }

      // required string servername = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_servername:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_servername()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->servername().data(), this->servername().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_seession;
        break;
      }

      // optional string seession = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_seession:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_seession()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->seession().data(), this->seession().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void CLogicLogin::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 cmd = 1 [default = 4096];
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->cmd(), output);
  }

  // required string servername = 2;
  if (has_servername()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->servername().data(), this->servername().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->servername(), output);
  }

  // optional string seession = 3;
  if (has_seession()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->seession().data(), this->seession().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->seession(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CLogicLogin::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 cmd = 1 [default = 4096];
  if (has_cmd()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->cmd(), target);
  }

  // required string servername = 2;
  if (has_servername()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->servername().data(), this->servername().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->servername(), target);
  }

  // optional string seession = 3;
  if (has_seession()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->seession().data(), this->seession().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->seession(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CLogicLogin::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 cmd = 1 [default = 4096];
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->cmd());
    }

    // required string servername = 2;
    if (has_servername()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->servername());
    }

    // optional string seession = 3;
    if (has_seession()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->seession());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CLogicLogin::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CLogicLogin* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CLogicLogin*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CLogicLogin::MergeFrom(const CLogicLogin& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_servername()) {
      set_servername(from.servername());
    }
    if (from.has_seession()) {
      set_seession(from.seession());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CLogicLogin::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CLogicLogin::CopyFrom(const CLogicLogin& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CLogicLogin::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void CLogicLogin::Swap(CLogicLogin* other) {
  if (other != this) {
    std::swap(cmd_, other->cmd_);
    std::swap(servername_, other->servername_);
    std::swap(seession_, other->seession_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CLogicLogin::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CLogicLogin_descriptor_;
  metadata.reflection = CLogicLogin_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int SLogicLogin::kCmdFieldNumber;
const int SLogicLogin::kErrFieldNumber;
#endif  // !_MSC_VER

SLogicLogin::SLogicLogin()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SLogicLogin::InitAsDefaultInstance() {
}

SLogicLogin::SLogicLogin(const SLogicLogin& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SLogicLogin::SharedCtor() {
  _cached_size_ = 0;
  cmd_ = 4096u;
  err_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SLogicLogin::~SLogicLogin() {
  SharedDtor();
}

void SLogicLogin::SharedDtor() {
  if (this != default_instance_) {
  }
}

void SLogicLogin::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SLogicLogin::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SLogicLogin_descriptor_;
}

const SLogicLogin& SLogicLogin::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_LogicServer_2eproto();
  return *default_instance_;
}

SLogicLogin* SLogicLogin::default_instance_ = NULL;

SLogicLogin* SLogicLogin::New() const {
  return new SLogicLogin;
}

void SLogicLogin::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    cmd_ = 4096u;
    err_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SLogicLogin::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 cmd = 1 [default = 4096];
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_err;
        break;
      }

      // optional uint32 err = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_err:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &err_)));
          set_has_err();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SLogicLogin::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 cmd = 1 [default = 4096];
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->cmd(), output);
  }

  // optional uint32 err = 2;
  if (has_err()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->err(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SLogicLogin::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 cmd = 1 [default = 4096];
  if (has_cmd()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->cmd(), target);
  }

  // optional uint32 err = 2;
  if (has_err()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->err(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SLogicLogin::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 cmd = 1 [default = 4096];
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->cmd());
    }

    // optional uint32 err = 2;
    if (has_err()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->err());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SLogicLogin::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SLogicLogin* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SLogicLogin*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SLogicLogin::MergeFrom(const SLogicLogin& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_err()) {
      set_err(from.err());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SLogicLogin::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SLogicLogin::CopyFrom(const SLogicLogin& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SLogicLogin::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void SLogicLogin::Swap(SLogicLogin* other) {
  if (other != this) {
    std::swap(cmd_, other->cmd_);
    std::swap(err_, other->err_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SLogicLogin::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SLogicLogin_descriptor_;
  metadata.reflection = SLogicLogin_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol

// @@protoc_insertion_point(global_scope)