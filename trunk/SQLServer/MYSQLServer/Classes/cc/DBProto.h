// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DBProto.proto

#ifndef PROTOBUF_DBProto_2eproto__INCLUDED
#define PROTOBUF_DBProto_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protocol {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_DBProto_2eproto();
void protobuf_AssignDesc_DBProto_2eproto();
void protobuf_ShutdownFile_DBProto_2eproto();

class DBUserInfo;

// ===================================================================

class DBUserInfo : public ::google::protobuf::Message {
 public:
  DBUserInfo();
  virtual ~DBUserInfo();

  DBUserInfo(const DBUserInfo& from);

  inline DBUserInfo& operator=(const DBUserInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const DBUserInfo& default_instance();

  void Swap(DBUserInfo* other);

  // implements Message ----------------------------------------------

  DBUserInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DBUserInfo& from);
  void MergeFrom(const DBUserInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string userid = 1;
  inline bool has_userid() const;
  inline void clear_userid();
  static const int kUseridFieldNumber = 1;
  inline const ::std::string& userid() const;
  inline void set_userid(const ::std::string& value);
  inline void set_userid(const char* value);
  inline void set_userid(const char* value, size_t size);
  inline ::std::string* mutable_userid();
  inline ::std::string* release_userid();
  inline void set_allocated_userid(::std::string* userid);

  // required string username = 2;
  inline bool has_username() const;
  inline void clear_username();
  static const int kUsernameFieldNumber = 2;
  inline const ::std::string& username() const;
  inline void set_username(const ::std::string& value);
  inline void set_username(const char* value);
  inline void set_username(const char* value, size_t size);
  inline ::std::string* mutable_username();
  inline ::std::string* release_username();
  inline void set_allocated_username(::std::string* username);

  // required uint32 sex = 3 [default = 1];
  inline bool has_sex() const;
  inline void clear_sex();
  static const int kSexFieldNumber = 3;
  inline ::google::protobuf::uint32 sex() const;
  inline void set_sex(::google::protobuf::uint32 value);

  // optional string ip = 4;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 4;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  inline void set_allocated_ip(::std::string* ip);

  // optional string address = 5;
  inline bool has_address() const;
  inline void clear_address();
  static const int kAddressFieldNumber = 5;
  inline const ::std::string& address() const;
  inline void set_address(const ::std::string& value);
  inline void set_address(const char* value);
  inline void set_address(const char* value, size_t size);
  inline ::std::string* mutable_address();
  inline ::std::string* release_address();
  inline void set_allocated_address(::std::string* address);

  // required uint32 gold = 6 [default = 4000];
  inline bool has_gold() const;
  inline void clear_gold();
  static const int kGoldFieldNumber = 6;
  inline ::google::protobuf::uint32 gold() const;
  inline void set_gold(::google::protobuf::uint32 value);

  // required uint32 diamon = 7 [default = 8];
  inline bool has_diamon() const;
  inline void clear_diamon();
  static const int kDiamonFieldNumber = 7;
  inline ::google::protobuf::uint32 diamon() const;
  inline void set_diamon(::google::protobuf::uint32 value);

  // required uint32 card = 8 [default = 1];
  inline bool has_card() const;
  inline void clear_card();
  static const int kCardFieldNumber = 8;
  inline ::google::protobuf::uint32 card() const;
  inline void set_card(::google::protobuf::uint32 value);

  // optional string code = 9;
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 9;
  inline const ::std::string& code() const;
  inline void set_code(const ::std::string& value);
  inline void set_code(const char* value);
  inline void set_code(const char* value, size_t size);
  inline ::std::string* mutable_code();
  inline ::std::string* release_code();
  inline void set_allocated_code(::std::string* code);

  // optional string token = 10;
  inline bool has_token() const;
  inline void clear_token();
  static const int kTokenFieldNumber = 10;
  inline const ::std::string& token() const;
  inline void set_token(const ::std::string& value);
  inline void set_token(const char* value);
  inline void set_token(const char* value, size_t size);
  inline ::std::string* mutable_token();
  inline ::std::string* release_token();
  inline void set_allocated_token(::std::string* token);

  // optional uint32 picid = 11 [default = 1];
  inline bool has_picid() const;
  inline void clear_picid();
  static const int kPicidFieldNumber = 11;
  inline ::google::protobuf::uint32 picid() const;
  inline void set_picid(::google::protobuf::uint32 value);

  // optional string unionid = 12;
  inline bool has_unionid() const;
  inline void clear_unionid();
  static const int kUnionidFieldNumber = 12;
  inline const ::std::string& unionid() const;
  inline void set_unionid(const ::std::string& value);
  inline void set_unionid(const char* value);
  inline void set_unionid(const char* value, size_t size);
  inline ::std::string* mutable_unionid();
  inline ::std::string* release_unionid();
  inline void set_allocated_unionid(::std::string* unionid);

  // optional string picurl = 13;
  inline bool has_picurl() const;
  inline void clear_picurl();
  static const int kPicurlFieldNumber = 13;
  inline const ::std::string& picurl() const;
  inline void set_picurl(const ::std::string& value);
  inline void set_picurl(const char* value);
  inline void set_picurl(const char* value, size_t size);
  inline ::std::string* mutable_picurl();
  inline ::std::string* release_picurl();
  inline void set_allocated_picurl(::std::string* picurl);

  // @@protoc_insertion_point(class_scope:protocol.DBUserInfo)
 private:
  inline void set_has_userid();
  inline void clear_has_userid();
  inline void set_has_username();
  inline void clear_has_username();
  inline void set_has_sex();
  inline void clear_has_sex();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_address();
  inline void clear_has_address();
  inline void set_has_gold();
  inline void clear_has_gold();
  inline void set_has_diamon();
  inline void clear_has_diamon();
  inline void set_has_card();
  inline void clear_has_card();
  inline void set_has_code();
  inline void clear_has_code();
  inline void set_has_token();
  inline void clear_has_token();
  inline void set_has_picid();
  inline void clear_has_picid();
  inline void set_has_unionid();
  inline void clear_has_unionid();
  inline void set_has_picurl();
  inline void clear_has_picurl();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* userid_;
  ::std::string* username_;
  ::std::string* ip_;
  ::google::protobuf::uint32 sex_;
  ::google::protobuf::uint32 gold_;
  ::std::string* address_;
  ::google::protobuf::uint32 diamon_;
  ::google::protobuf::uint32 card_;
  ::std::string* code_;
  ::std::string* token_;
  ::std::string* unionid_;
  ::std::string* picurl_;
  ::google::protobuf::uint32 picid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(13 + 31) / 32];

  friend void  protobuf_AddDesc_DBProto_2eproto();
  friend void protobuf_AssignDesc_DBProto_2eproto();
  friend void protobuf_ShutdownFile_DBProto_2eproto();

  void InitAsDefaultInstance();
  static DBUserInfo* default_instance_;
};
// ===================================================================


// ===================================================================

// DBUserInfo

// required string userid = 1;
inline bool DBUserInfo::has_userid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DBUserInfo::set_has_userid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DBUserInfo::clear_has_userid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DBUserInfo::clear_userid() {
  if (userid_ != &::google::protobuf::internal::kEmptyString) {
    userid_->clear();
  }
  clear_has_userid();
}
inline const ::std::string& DBUserInfo::userid() const {
  return *userid_;
}
inline void DBUserInfo::set_userid(const ::std::string& value) {
  set_has_userid();
  if (userid_ == &::google::protobuf::internal::kEmptyString) {
    userid_ = new ::std::string;
  }
  userid_->assign(value);
}
inline void DBUserInfo::set_userid(const char* value) {
  set_has_userid();
  if (userid_ == &::google::protobuf::internal::kEmptyString) {
    userid_ = new ::std::string;
  }
  userid_->assign(value);
}
inline void DBUserInfo::set_userid(const char* value, size_t size) {
  set_has_userid();
  if (userid_ == &::google::protobuf::internal::kEmptyString) {
    userid_ = new ::std::string;
  }
  userid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_userid() {
  set_has_userid();
  if (userid_ == &::google::protobuf::internal::kEmptyString) {
    userid_ = new ::std::string;
  }
  return userid_;
}
inline ::std::string* DBUserInfo::release_userid() {
  clear_has_userid();
  if (userid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = userid_;
    userid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_userid(::std::string* userid) {
  if (userid_ != &::google::protobuf::internal::kEmptyString) {
    delete userid_;
  }
  if (userid) {
    set_has_userid();
    userid_ = userid;
  } else {
    clear_has_userid();
    userid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string username = 2;
inline bool DBUserInfo::has_username() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DBUserInfo::set_has_username() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DBUserInfo::clear_has_username() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DBUserInfo::clear_username() {
  if (username_ != &::google::protobuf::internal::kEmptyString) {
    username_->clear();
  }
  clear_has_username();
}
inline const ::std::string& DBUserInfo::username() const {
  return *username_;
}
inline void DBUserInfo::set_username(const ::std::string& value) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::kEmptyString) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void DBUserInfo::set_username(const char* value) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::kEmptyString) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void DBUserInfo::set_username(const char* value, size_t size) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::kEmptyString) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_username() {
  set_has_username();
  if (username_ == &::google::protobuf::internal::kEmptyString) {
    username_ = new ::std::string;
  }
  return username_;
}
inline ::std::string* DBUserInfo::release_username() {
  clear_has_username();
  if (username_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = username_;
    username_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_username(::std::string* username) {
  if (username_ != &::google::protobuf::internal::kEmptyString) {
    delete username_;
  }
  if (username) {
    set_has_username();
    username_ = username;
  } else {
    clear_has_username();
    username_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 sex = 3 [default = 1];
inline bool DBUserInfo::has_sex() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void DBUserInfo::set_has_sex() {
  _has_bits_[0] |= 0x00000004u;
}
inline void DBUserInfo::clear_has_sex() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void DBUserInfo::clear_sex() {
  sex_ = 1u;
  clear_has_sex();
}
inline ::google::protobuf::uint32 DBUserInfo::sex() const {
  return sex_;
}
inline void DBUserInfo::set_sex(::google::protobuf::uint32 value) {
  set_has_sex();
  sex_ = value;
}

// optional string ip = 4;
inline bool DBUserInfo::has_ip() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void DBUserInfo::set_has_ip() {
  _has_bits_[0] |= 0x00000008u;
}
inline void DBUserInfo::clear_has_ip() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void DBUserInfo::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& DBUserInfo::ip() const {
  return *ip_;
}
inline void DBUserInfo::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void DBUserInfo::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void DBUserInfo::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* DBUserInfo::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_ip(::std::string* ip) {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    delete ip_;
  }
  if (ip) {
    set_has_ip();
    ip_ = ip;
  } else {
    clear_has_ip();
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string address = 5;
inline bool DBUserInfo::has_address() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void DBUserInfo::set_has_address() {
  _has_bits_[0] |= 0x00000010u;
}
inline void DBUserInfo::clear_has_address() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void DBUserInfo::clear_address() {
  if (address_ != &::google::protobuf::internal::kEmptyString) {
    address_->clear();
  }
  clear_has_address();
}
inline const ::std::string& DBUserInfo::address() const {
  return *address_;
}
inline void DBUserInfo::set_address(const ::std::string& value) {
  set_has_address();
  if (address_ == &::google::protobuf::internal::kEmptyString) {
    address_ = new ::std::string;
  }
  address_->assign(value);
}
inline void DBUserInfo::set_address(const char* value) {
  set_has_address();
  if (address_ == &::google::protobuf::internal::kEmptyString) {
    address_ = new ::std::string;
  }
  address_->assign(value);
}
inline void DBUserInfo::set_address(const char* value, size_t size) {
  set_has_address();
  if (address_ == &::google::protobuf::internal::kEmptyString) {
    address_ = new ::std::string;
  }
  address_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_address() {
  set_has_address();
  if (address_ == &::google::protobuf::internal::kEmptyString) {
    address_ = new ::std::string;
  }
  return address_;
}
inline ::std::string* DBUserInfo::release_address() {
  clear_has_address();
  if (address_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = address_;
    address_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_address(::std::string* address) {
  if (address_ != &::google::protobuf::internal::kEmptyString) {
    delete address_;
  }
  if (address) {
    set_has_address();
    address_ = address;
  } else {
    clear_has_address();
    address_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 gold = 6 [default = 4000];
inline bool DBUserInfo::has_gold() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void DBUserInfo::set_has_gold() {
  _has_bits_[0] |= 0x00000020u;
}
inline void DBUserInfo::clear_has_gold() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void DBUserInfo::clear_gold() {
  gold_ = 4000u;
  clear_has_gold();
}
inline ::google::protobuf::uint32 DBUserInfo::gold() const {
  return gold_;
}
inline void DBUserInfo::set_gold(::google::protobuf::uint32 value) {
  set_has_gold();
  gold_ = value;
}

// required uint32 diamon = 7 [default = 8];
inline bool DBUserInfo::has_diamon() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void DBUserInfo::set_has_diamon() {
  _has_bits_[0] |= 0x00000040u;
}
inline void DBUserInfo::clear_has_diamon() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void DBUserInfo::clear_diamon() {
  diamon_ = 8u;
  clear_has_diamon();
}
inline ::google::protobuf::uint32 DBUserInfo::diamon() const {
  return diamon_;
}
inline void DBUserInfo::set_diamon(::google::protobuf::uint32 value) {
  set_has_diamon();
  diamon_ = value;
}

// required uint32 card = 8 [default = 1];
inline bool DBUserInfo::has_card() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void DBUserInfo::set_has_card() {
  _has_bits_[0] |= 0x00000080u;
}
inline void DBUserInfo::clear_has_card() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void DBUserInfo::clear_card() {
  card_ = 1u;
  clear_has_card();
}
inline ::google::protobuf::uint32 DBUserInfo::card() const {
  return card_;
}
inline void DBUserInfo::set_card(::google::protobuf::uint32 value) {
  set_has_card();
  card_ = value;
}

// optional string code = 9;
inline bool DBUserInfo::has_code() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void DBUserInfo::set_has_code() {
  _has_bits_[0] |= 0x00000100u;
}
inline void DBUserInfo::clear_has_code() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void DBUserInfo::clear_code() {
  if (code_ != &::google::protobuf::internal::kEmptyString) {
    code_->clear();
  }
  clear_has_code();
}
inline const ::std::string& DBUserInfo::code() const {
  return *code_;
}
inline void DBUserInfo::set_code(const ::std::string& value) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  code_->assign(value);
}
inline void DBUserInfo::set_code(const char* value) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  code_->assign(value);
}
inline void DBUserInfo::set_code(const char* value, size_t size) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  code_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_code() {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  return code_;
}
inline ::std::string* DBUserInfo::release_code() {
  clear_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = code_;
    code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_code(::std::string* code) {
  if (code_ != &::google::protobuf::internal::kEmptyString) {
    delete code_;
  }
  if (code) {
    set_has_code();
    code_ = code;
  } else {
    clear_has_code();
    code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string token = 10;
inline bool DBUserInfo::has_token() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void DBUserInfo::set_has_token() {
  _has_bits_[0] |= 0x00000200u;
}
inline void DBUserInfo::clear_has_token() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void DBUserInfo::clear_token() {
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    token_->clear();
  }
  clear_has_token();
}
inline const ::std::string& DBUserInfo::token() const {
  return *token_;
}
inline void DBUserInfo::set_token(const ::std::string& value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(value);
}
inline void DBUserInfo::set_token(const char* value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(value);
}
inline void DBUserInfo::set_token(const char* value, size_t size) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_token() {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  return token_;
}
inline ::std::string* DBUserInfo::release_token() {
  clear_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = token_;
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_token(::std::string* token) {
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    delete token_;
  }
  if (token) {
    set_has_token();
    token_ = token;
  } else {
    clear_has_token();
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional uint32 picid = 11 [default = 1];
inline bool DBUserInfo::has_picid() const {
  return (_has_bits_[0] & 0x00000400u) != 0;
}
inline void DBUserInfo::set_has_picid() {
  _has_bits_[0] |= 0x00000400u;
}
inline void DBUserInfo::clear_has_picid() {
  _has_bits_[0] &= ~0x00000400u;
}
inline void DBUserInfo::clear_picid() {
  picid_ = 1u;
  clear_has_picid();
}
inline ::google::protobuf::uint32 DBUserInfo::picid() const {
  return picid_;
}
inline void DBUserInfo::set_picid(::google::protobuf::uint32 value) {
  set_has_picid();
  picid_ = value;
}

// optional string unionid = 12;
inline bool DBUserInfo::has_unionid() const {
  return (_has_bits_[0] & 0x00000800u) != 0;
}
inline void DBUserInfo::set_has_unionid() {
  _has_bits_[0] |= 0x00000800u;
}
inline void DBUserInfo::clear_has_unionid() {
  _has_bits_[0] &= ~0x00000800u;
}
inline void DBUserInfo::clear_unionid() {
  if (unionid_ != &::google::protobuf::internal::kEmptyString) {
    unionid_->clear();
  }
  clear_has_unionid();
}
inline const ::std::string& DBUserInfo::unionid() const {
  return *unionid_;
}
inline void DBUserInfo::set_unionid(const ::std::string& value) {
  set_has_unionid();
  if (unionid_ == &::google::protobuf::internal::kEmptyString) {
    unionid_ = new ::std::string;
  }
  unionid_->assign(value);
}
inline void DBUserInfo::set_unionid(const char* value) {
  set_has_unionid();
  if (unionid_ == &::google::protobuf::internal::kEmptyString) {
    unionid_ = new ::std::string;
  }
  unionid_->assign(value);
}
inline void DBUserInfo::set_unionid(const char* value, size_t size) {
  set_has_unionid();
  if (unionid_ == &::google::protobuf::internal::kEmptyString) {
    unionid_ = new ::std::string;
  }
  unionid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_unionid() {
  set_has_unionid();
  if (unionid_ == &::google::protobuf::internal::kEmptyString) {
    unionid_ = new ::std::string;
  }
  return unionid_;
}
inline ::std::string* DBUserInfo::release_unionid() {
  clear_has_unionid();
  if (unionid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = unionid_;
    unionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_unionid(::std::string* unionid) {
  if (unionid_ != &::google::protobuf::internal::kEmptyString) {
    delete unionid_;
  }
  if (unionid) {
    set_has_unionid();
    unionid_ = unionid;
  } else {
    clear_has_unionid();
    unionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string picurl = 13;
inline bool DBUserInfo::has_picurl() const {
  return (_has_bits_[0] & 0x00001000u) != 0;
}
inline void DBUserInfo::set_has_picurl() {
  _has_bits_[0] |= 0x00001000u;
}
inline void DBUserInfo::clear_has_picurl() {
  _has_bits_[0] &= ~0x00001000u;
}
inline void DBUserInfo::clear_picurl() {
  if (picurl_ != &::google::protobuf::internal::kEmptyString) {
    picurl_->clear();
  }
  clear_has_picurl();
}
inline const ::std::string& DBUserInfo::picurl() const {
  return *picurl_;
}
inline void DBUserInfo::set_picurl(const ::std::string& value) {
  set_has_picurl();
  if (picurl_ == &::google::protobuf::internal::kEmptyString) {
    picurl_ = new ::std::string;
  }
  picurl_->assign(value);
}
inline void DBUserInfo::set_picurl(const char* value) {
  set_has_picurl();
  if (picurl_ == &::google::protobuf::internal::kEmptyString) {
    picurl_ = new ::std::string;
  }
  picurl_->assign(value);
}
inline void DBUserInfo::set_picurl(const char* value, size_t size) {
  set_has_picurl();
  if (picurl_ == &::google::protobuf::internal::kEmptyString) {
    picurl_ = new ::std::string;
  }
  picurl_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBUserInfo::mutable_picurl() {
  set_has_picurl();
  if (picurl_ == &::google::protobuf::internal::kEmptyString) {
    picurl_ = new ::std::string;
  }
  return picurl_;
}
inline ::std::string* DBUserInfo::release_picurl() {
  clear_has_picurl();
  if (picurl_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = picurl_;
    picurl_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBUserInfo::set_allocated_picurl(::std::string* picurl) {
  if (picurl_ != &::google::protobuf::internal::kEmptyString) {
    delete picurl_;
  }
  if (picurl) {
    set_has_picurl();
    picurl_ = picurl;
  } else {
    clear_has_picurl();
    picurl_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_DBProto_2eproto__INCLUDED
