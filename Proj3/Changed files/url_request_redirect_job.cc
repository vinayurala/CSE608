// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/url_request/url_request_redirect_job.h"

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/instrumenter.h"
#include "base/message_loop/message_loop.h"
#include "net/base/load_timing_info.h"

#include <string>
#include <android/log.h>

namespace net {

URLRequestRedirectJob::URLRequestRedirectJob(URLRequest* request,
                                             NetworkDelegate* network_delegate,
                                             const GURL& redirect_destination,
                                             StatusCode http_status_code)
    : URLRequestJob(request, network_delegate),
      redirect_destination_(redirect_destination),
      http_status_code_(http_status_code),
      weak_factory_(this) {}

void URLRequestRedirectJob::Start() {
  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&URLRequestRedirectJob::StartAsync,
                 weak_factory_.GetWeakPtr()));
}

bool URLRequestRedirectJob::IsRedirectResponse(GURL* location,
                                               int* http_status_code) {
  *location = redirect_destination_;
  *http_status_code = http_status_code_;
  
  std::string redirect_dest_string = std::string();
  if (location->SchemeIsSecure())
    redirect_dest_string = "https://";
  else
    redirect_dest_string = "http://";
  
  redirect_dest_string += location->host();
  if (location->port() != "-1")
    redirect_dest_string += location->port();

  redirect_dest_string += location->path();
  redirect_dest_string += location->query();
  redirect_dest_string += location->ref();
  redirect_dest_string = "Redirection request received. Redirecting to: " 
    + redirect_dest_string;


  instrumenter i_obj;
  bool ret = i_obj.log_instrument_data("verbose", "Proj3", redirect_dest_string);
  if (!ret)
    __android_log_write(ANDROID_LOG_ERROR, "chromium", "Failed to log instrumented data; missing information");

  return true;
}

URLRequestRedirectJob::~URLRequestRedirectJob() {}

void URLRequestRedirectJob::StartAsync() {
  receive_headers_end_ = base::TimeTicks::Now();
  NotifyHeadersComplete();
}

void URLRequestRedirectJob::GetLoadTimingInfo(
    LoadTimingInfo* load_timing_info) const {
  // Set send_start and send_end to receive_headers_end_ to keep consistent
  // with network cache behavior.
  load_timing_info->send_start = receive_headers_end_;
  load_timing_info->send_end = receive_headers_end_;
  load_timing_info->receive_headers_end = receive_headers_end_;
}

}  // namespace net
