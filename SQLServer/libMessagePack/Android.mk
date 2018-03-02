LOCAL_PATH := $(call my-dir)
#定义函数，递归返回给定目录下所有目录
all_directory = \
	$(eval item_all = $(wildcard $1/*))\
	$(eval item_all = $(item_all:%.cpp=))\
	$(eval item_all = $(item_all:%.cc=))\
	$(eval item_all = $(item_all:%.mk=))\
	$(eval item_all = $(item_all:%.a=))\
	$(eval item_all = $(item_all:%.so=))\
	$(eval item_all = $(item_all:%.o=))\
	$(eval item_all = $(item_all:%.d=))\
	$(eval item_all = $(item_all:%.obj=))\
	$(eval item_all = $(item_all:%.txt=))\
	$(eval item_all = $(item_all:%.sh=))\
	$(eval item_all = $(item_all:%.proto=))\
	$(eval item_all = $(item_all:%.c=))\
	$(eval item_all = $(item_all:%.cmake=))\
	$(eval item_all = $(item_all:%.tlog=))\
	$(eval item_all = $(item_all:%.log=))\
	$(eval item_all = $(item_all:%.idb=))\
	$(eval item_all = $(item_all:%.pdb=))\
	$(eval item_all = $(item_all:%.html=))\
	$(eval item_all = $(item_all:%.jce=))\
	$(eval item_all = $(item_all:%.mm=))\
	$(eval item_all = $(item_all:%.m=))\
	$(eval item_all = $(item_all:%.lib=))\
	$(eval item_all = $(item_all:%.zip=))\
	$(eval item_all = $(item_all:%.sln=))\
	$(eval item_all = $(item_all:%.vcproj=))\
	$(eval item_all = $(item_all:%.pbxproj=))\
	$(eval item_all = $(item_all:%.xcworkspacedata=))\
	$(eval item_all = $(item_all:%.xcuserstate=))\
	$(eval item_all = $(item_all:%.lastbuildstate=))\
	$(eval item_all = $(item_all:%.xcscheme=))\
	$(eval item_all = $(item_all:%.filters=))\
	$(eval item_all = $(item_all:%.user=))\
	$(eval item_all = $(item_all:%.vcxproj=))\
	$(eval item_all = $(item_all:%.pbxproj=))\
	$(eval item_all = $(item_all:%.bat=))\
	$(eval item_all = $(item_all:%.tlog=))\
	$(eval item_all = $(item_all:%.mk=))\
	$(eval item_all = $(item_all:%.jar=))\
	$(eval item_all = $(item_all:%.suo=))\
	$(eval item_all = $(item_all:%.sdf=))\
	$(eval item_all = $(item_all:%.properties=))\
	$(eval item_all = $(item_all:%.plist=))\
	$(eval item_all = $(item_all:%.xml=))\
	$(eval item_all = $(item_all:%.classpath=))\
	$(eval item_all = $(item_all:%.project=))\
	$(eval item_all = $(item_all:%.doc=))\
	$(eval item_all = $(item_all:%.keystore=))\
	$(eval item_all = $(item_all:%.java=))\
	$(eval item_all = $(item_all:%.png=))\
	$(eval item_all = $(item_all:%.jpeg=))\
	$(eval item_all = $(item_all:%.hpp=))\
	$(eval item_all = $(item_all:%.h=))$1/ $(item_all)\
	$(foreach item, $(item_all) $(),\
		$(call all_directory, $(item))\
	)
#定义函数，递归返回给定目录下所有C源文件
all_src_c_files_recursively = \
	$(eval src_files = $(wildcard $1/*.c)) \
	$(eval src_files = $(src_files:$(LOCAL_PATH)/%=%))$(src_files) \
	$(eval item_all = $(wildcard $1/*)) \
	$(foreach item, $(item_all) $(),\
	$(eval item := $(item:%.c=%)) \
	$(call all_src_c_files_recursively, $(item))\
)	
	
#定义函数，递归返回给定目录下所有C++源文件
all_src_files_recursively = \
	$(eval src_files = $(wildcard $1/*.cpp)) \
	$(eval src_files = $(src_files:$(LOCAL_PATH)/%=%))$(src_files) \
	$(eval item_all = $(wildcard $1/*)) \
	$(foreach item, $(item_all) $(),\
	$(eval item := $(item:%.cpp=%)) \
	$(call all_src_files_recursively, $(item))\
)
#定义函数，递归返回给定目录下所有CC源文件
all_srcc_files_recursively = \
	$(eval src_files = $(wildcard $1/*.cc)) \
	$(eval src_files = $(src_files:$(LOCAL_PATH)/%=%))$(src_files) \
	$(eval item_all = $(wildcard $1/*)) \
	$(foreach item, $(item_all) $(),\
	$(eval item := $(item:%.cc=%)) \
	$(call all_srcc_files_recursively, $(item))\
)

#开始编译
include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cc .cpp .c

LOCAL_MODULE := messagepack_static
LOCAL_MODULE_FILENAME := libMsgPack

LOCAL_SRC_FILES := $(call all_src_files_recursively, $(LOCAL_PATH))
LOCAL_SRC_FILES += $(call all_src_c_files_recursively,$(LOCAL_PATH))
LOCAL_SRC_FILES += $(call all_srcc_files_recursively,$(LOCAL_PATH))

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CPPFLAGS := -fexceptions


include $(BUILD_STATIC_LIBRARY)

