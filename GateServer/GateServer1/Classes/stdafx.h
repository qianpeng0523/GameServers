// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include<algorithm>
#include<vector>
#include<iostream>

#include "Object.h"
#include <map>
#include <set>
#include <string.h>
#include <xstring>
#include "MD5.h"
#include "XXIconv.h"
#include "CProtocol.h"
#include "YMSocketData.h"
#include "Poco/Thread.h"
#include "LogicServer.h"
#include "ccEvent.h"
#include "Login.h"
#include "ConfigProto.h"
#include "Hall.h"
#include "redis.h"
#include "RoomLogic.h"

using namespace std;
using namespace protocol;
using namespace Poco;

#define HEADLEN 10
#define ZHONGFABAI
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../../Debug/libiconv.lib") 
#pragma comment(lib,"../../Debug/libprotobuf.lib")
#pragma comment(lib,"../../Debug/libMessagePack.lib")
#pragma comment(lib,"../../Debug/libcrypto.lib")
#pragma comment(lib,"../../../redis/Debug/hiredis.lib")
#endif

//#define sqlhttp "http://192.168.50.151:8080/"
//#define sqlhttp "http://192.168.1.101:8080/"
#define sqlhttp "http://www.lesharecs.com:8080/"
#define LOGIC_TOKEN "kashdkiuySKJHFKS312skfhksuiie2342"
#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"

#define ZERO_STR '&'
//#define FENG_LIAN 0


/*******微信支付定义********/
#define APPID "wxd5d25d59ae13ac95"
#define MCHID "1501106141"
#define INITNONCEID "18880000"
#define NOTIFYURL "http://www.lesharecs.com:8082/wxpaycallback"
#define TRADETYPE "APP"
#define INITNO "00000000"
#define APPSECRET "42eded380f7f410e1556a1a94cfaa096"

/***********支付宝支付**************/
//#define ALIAPPID "2017112900248589"
#define ALIAPPID "2016091500519796"
#define ALIPRIKEY "MIIEowIBAAKCAQEAuya9g8UZu1yVvULELwTS9ExjhTDICqR50kYPxmlnhD+3xI4GS1S0rBRd+bCPAcn4sAh4oVW1h2mBE0twS9mnjjqHExjZ+NcDxlL3yub/F8I4upfWuK623Oiy58tXV3gDN3bAajryPSobZ7iqOmQtQH01D95E57nmYtB/vjuwYD2R2UtNI21YTN/R+EmWLoBIv6HIXDfh0hHOJhbgfB4iSbwmjA1507X59HswUEh/tV3Qoc9yQX8P2U/SAoBD2wy3nn/Q8jJCavksqbcIZ0PQ5Dl3tEp/xjkG9pmXoGtT5xsHGt66RqXQuhGlUfP5H6NX7Q1tfuvV4U1pZaZDhvDH4QIDAQABAoIBAE2OjWUJDlPV13v6KkDt99ojue5dO9klNhLM1Aa+cZadrTM4RCV9se3J9vPdljYAlB2nNQ5u7W5za3kqfxv/0+8eFBH274Wu3TKX+XZNahNXepjhCau5Q7CiglmKq+RkqLiYbr7XNMHiLD88iNbv4szh1laAHwAEyxPSIc4dAEexJdZDkxV7S4qw4XJ0cSKnvEUFdBQ9yYhB14usouGFf5RZNVO6/Pt7b+17IsJrzFx3Y2aP9vFgdTZeL0b+59Nuf30SKouzc+0FwOOPUhk9OyV+K+dJso98p0MykBSfdacVlWTwymYVNZw6VTxfl+kxi+CiuBhh8a6D7wwQ0peMQV0CgYEA8snc74D1I4vX4UlkBeRTAJdmsi//FBu31JX+p0mxDEeuf7m/DFbAZq6Z9Xiok2i4HLsunYhX9uj+Hj/Exu4Se+sWn/SUQLKnVlV9qN7Uz4dfFBVJr9hXVTkWEYubRIn1DIbeaY0iY1pM9CAuQ0s5s1Tiu18PDld7RDqIK1pqu3cCgYEAxVXUb7QmK+5zZ4XPWbjNIzznklDQlSzGnteqEPcm6wbrTApg5VgoBOw1vaj9OS9AAdXWTyUCpqtYdsXel40HBmaokNg724pS7nTfq155untj5qNG4ubnhZv6URFgaCHuoml9BIuOWGzrCFvyGprVornxn8L/UzVGBdYkxXBMPWcCgYAlzE2ML/x6Izxhw4ZB8JEHh4qidRx2KfaNryhtLOjWa5Z8JhPbPOpJUid/8t/zyxrWxkF0nxRqyU0+sljVS50vE6N7FZ27ENVCeJ7OmppxELc5rjLBoKHAD9oisyRiIEQ5Rgn8Ts+qvhU68IOjUGPUSoPd/eefKPl03tPyQ+pX8wKBgGTzlr0ahRieYQ2Jx9zXjKJFSxZwH3ThNi8Wl1schR1hKch52r4ZTkfOa2O+auTxRiZqNbvBbEzFhXklms0fCYP9yqhm5qBWcDyrL62kpFOBQkdAKkhz5edVN0Z3xEEhcspFev5zQnv8JHTfTMjNgxM/WtvnTuVLcKoVtjQHUglrAoGBAIs7FxgJuRGQmfwpGg8VGKyJWh9VP/uKpBDfRE+Z7t+oZnYZULctU4zNVD1IiHru5H3fgG6qR+p5OMjlA/7iSnvd58AZs8MJVjEENMrpKiHLWVQ44k18lLsnGvF3cnFQDC1FGiNHA3LJae4JaVJzab7vFziBOsbnxlFMVWoZQlf2"
#define ALIPRIKEY_0 "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC7Jr2DxRm7XJW9QsQvBNL0TGOFMMgKpHnSRg/GaWeEP7fEjgZLVLSsFF35sI8ByfiwCHihVbWHaYETS3BL2aeOOocTGNn41wPGUvfK5v8Xwji6l9a4rrbc6LLny1dXeAM3dsBqOvI9KhtnuKo6ZC1AfTUP3kTnueZi0H++O7BgPZHZS00jbVhM39H4SZYugEi/ochcN+HSEc4mFuB8HiJJvCaMDXnTtfn0ezBQSH+1XdChz3JBfw/ZT9ICgEPbDLeef9DyMkJq+SyptwhnQ9DkOXe0Sn/GOQb2mZega1PnGwca3rpGpdC6EaVR8/kfo1ftDW1+69XhTWllpkOG8MfhAgMBAAECggEATY6NZQkOU9XXe/oqQO332iO57l072SU2EszUBr5xlp2tMzhEJX2x7cn2892WNgCUHac1Dm7tbnNreSp/G//T7x4UEfbvha7dMpf5dk1qE1d6mOEJq7lDsKKCWYqr5GSouJhuvtc0weIsPzyI1u/izOHWVoAfAATLE9Ihzh0AR7El1kOTFXtLirDhcnRxIqe8RQV0FD3JiEHXi6yi4YV/lFk1U7r8+3tv7XsiwmvMXHdjZo/28WB1Nl4vRv7n025/fRIqi7Nz7QXA449SGT07JX4r50myj3ynQzKQFJ91pxWVZPDKZhU1nDpVPF+X6TGL4KK4GGHxroPvDBDSl4xBXQKBgQDyydzvgPUji9fhSWQF5FMAl2ayL/8UG7fUlf6nSbEMR65/ub8MVsBmrpn1eKiTaLgcuy6diFf26P4eP8TG7hJ76xaf9JRAsqdWVX2o3tTPh18UFUmv2FdVORYRi5tEifUMht5pjSJjWkz0IC5DSzmzVOK7Xw8OV3tEOogrWmq7dwKBgQDFVdRvtCYr7nNnhc9ZuM0jPOeSUNCVLMae16oQ9ybrButMCmDlWCgE7DW9qP05L0AB1dZPJQKmq1h2xd6XjQcGZqiQ2DvbilLudN+rXnm6e2Pmo0bi5ueFm/pREWBoIe6iaX0Ei45YbOsIW/IamtWiufGfwv9TNUYF1iTFcEw9ZwKBgCXMTYwv/HojPGHDhkHwkQeHiqJ1HHYp9o2vKG0s6NZrlnwmE9s86klSJ3/y3/PLGtbGQXSfFGrJTT6yWNVLnS8To3sVnbsQ1UJ4ns6amnEQtzmuMsGgocAP2iKzJGIgRDlGCfxOz6q+FTrwg6NQY9RKg939558o+XTe0/JD6lfzAoGAZPOWvRqFGJ5hDYnH3NeMokVLFnAfdOE2LxaXWxyFHWEpyHnavhlOR85rY75q5PFGJmo1u8FsTMWFeSWazR8Jg/3KqGbmoFZwPKsvraSkU4FCR0AqSHPl51U3RnfEQSFyykV6/nNCe/wkdN9MyM2DEz9a2+dO5UtwqhW2NAdSCWsCgYEAizsXGAm5EZCZ/CkaDxUYrIlaH1U/+4qkEN9ET5nu36hmdhlQty1TjM1UPUiIeu7kfd+AbqpH6nk4yOUD/uJKe93nwBmzwwlWMQQ0yukqIctZVDjiTXyUuyca8XdycVAMLUUaI0cDcslp7glpUnNpvu8XOIE6xufGUUxVahlCV/Y="
//#define ALIPUBKEY "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlQStQeveZy9x87UEx35kZas6sew/tQw3xFtEsE9GiQAnza6GBRZElior8xL+uVTG21TfMbEW2DJDEYEB2od7qg9X4sRnp4/8gTM+eUHWwU5Ydt8Kj98EOnVGKvi/iAPsYMoyJUn98ElHO3ZcSvUMCy/hYi/XzVntP6YyCd0DhB7mtdIFQEjQt/bGWjM+vDUKFA+lAzQs1FM5+kzZM4uye93dmNsnrwD5trs24JZJVhngTPeblO8YkhAxeAVP7bnu9ge1zjLpX5WcLH4D39cXqdA0C7bz7cseXJc+KxwbBIWiWBhxKVY9prW0e+vxCLUjpXXEJhb4SpUubhaT0dFiIwIDAQAB"
#define ALIPUBKEY "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmrORx+2IxzCVjpcUCvd6hMwihur9/d/lQAUwJ/RNvOQZFEya5nOHt2VC0KFfIH7pylPAlXEfFwSrbHR2X1Tc2qI692RE1qcdabI/aCDU2GXVgl87MapvJNwIiYjq481n25lkljCmXRMy9Zi9Grt8W3WPE+cguiKscjvAoGfmtPXfilvZOeObWlQwWCGlCzpAVG+uqIT2vB9odNMRdvHiqMnP+TQIZxAHwxweEgmV1KTfd19+B2n+i8tyTXFjveYcNfbRrLnuaO0zc6sx9Qw84mzV7rEQsoyw5HfeYuND8c9qA6+obbHrJrizRzAWF74IiYzrrHaIAvOTUOBvkc+7uQIDAQAB"
//#define ALIPAYURL "https://openapi.alipay.com/gateway.do"
#define ALIPAYURL "https://openapi.alipaydev.com/gateway.do"
#define CHARSET "utf-8"

#define KONGGE "^"
#define MAXSTAMP 128