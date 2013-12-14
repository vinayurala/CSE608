#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <ctime>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>

#include "instrumenter.h"

using namespace std;

string instrumenter::touch_state_map[] = {"Undefined",
					  "Released",
					  "Pressed",
					  "Moved",
					  "Stationary",
					  "Moved"};
					 
instrumenter::instrumenter()
{
  this->populate_mime_type_map();
}

instrumenter::instrumenter(string log_file_dir,
			   string log_file_name_prefix, 
			   string log_file_name_extension)
{
  this->populate_mime_type_map();
}

instrumenter::~instrumenter()
{
}

bool instrumenter::log_instrument_data(string log_level, string tag, string log_msg,
				       pid_t pid, string curr_time) 
{

  if (log_level.empty() || tag.empty() || log_msg.empty())
    return false;
  
  string proc_name;

  if (pid == -1) {
    pid = getpid();
    proc_name = "Browser";
  }
  else 
    proc_name = "Renderer";

  if (curr_time == "")
    curr_time = get_time();

  if (!((InstrumenterDatabase::get_db_instance())->add_row(proc_name, 
							   curr_time, tag, log_msg, pid)))
    return false;

  return true;
}

string instrumenter::get_time(time_t now)
{
  if (!now)
    now = time(0);
  struct tm tm_struct;
  char buf[80];
  tm_struct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tm_struct);
  return buf;
}

void instrumenter::collect_and_log_download_data(GURL source,
						 GURL referrer,
						 base::FilePath file_path,
						 string file_name,
						 int64 total_size)
{
  string src_url_str = source.spec();
  string referrer_url_str = referrer.spec();
  string scratch = "Downloading a new file from: " + src_url_str + ", referred by: " + 
    referrer_url_str + ". Suggested file path = " + file_path.value() + file_name + 
    ". File size = " + stringify(total_size);

  log_instrument_data("verbose", "Download", scratch);
}

void instrumenter::collect_and_log_touch_data(int state, int x_coord, int y_coord)
{
  string x_coord_str = stringify(x_coord);
  string y_coord_str = stringify(y_coord);
  string state_str = touch_state_map[state];

  string log_msg = "Detected touch state: " + state_str + ". X co-ordinate: " +
    x_coord_str + ", Y co-ordinate: " + y_coord_str;
  
  log_instrument_data("verbose", "Touch data", log_msg);
}

void instrumenter::collect_and_log_cookie_data(GURL url, string name,
					       string value, 
					       base::Time expiration_time)
{
  string url_str = url.spec();
  string expiration_time_str = get_time(expiration_time.ToTimeT());
  string log_msg = "Storing a cookie from URL: " + url_str + ". Name = " + name +
    " Value = " + value + ". Expiration Time = " + expiration_time_str;  

  log_instrument_data("verbose", "Cookie", log_msg);

}

void instrumenter::collect_and_log_cookie_line(GURL url, string cookie_line)
{
  string log_msg = "Storing a new cookie from URL: " + url.spec() + ". Details: " + 
    cookie_line;

  log_instrument_data("verbose", "Cookie", log_msg);
}

void instrumenter::populate_mime_type_map()
{
  mime_type_map.insert(std::pair<std::string, std::string>("application/pdf", "PDF"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/octet-stream ", "Binary"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/json", "JSON"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/xml", "XML"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/zip", "ZIP"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/gzip", "GZIP"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/atom+xml", "Atom feeds"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/ecmascript", "ECMAScriptavaScript "));
  mime_type_map.insert(std::pair<std::string, std::string>("application/EDI-X12", "EDI X12 data"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/EDIFACT", "EDI EDIFACT data"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/javascript", "ECMAScript/JavaScript;"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/ogg", 
							   "Ogg, a multimedia bitstream container format."));
  mime_type_map.insert(std::pair<std::string, std::string>("application/postscript", "PostScript"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/rdf+xml", 
							   "Resource Description Framework"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/rss+xml", "RSS feeds"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/soap+xml", "SOAP"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/font-woff", "Web Open Font Format;"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/xhtml+xml", "XHTML"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/xml-dtd", "DTD files"));
  mime_type_map.insert(std::pair<std::string, std::string>("application/xop+xml", "XOP"));

  mime_type_map.insert(std::pair<std::string, std::string>("image/png", "Image (PNG)"));
  mime_type_map.insert(std::pair<std::string, std::string>("image/jpeg", "Image (JPEG)"));
  mime_type_map.insert(std::pair<std::string, std::string>("image/gif", "Image (GIF)"));
  mime_type_map.insert(std::pair<std::string, std::string>("image/pjpeg", "JPEG JFIF image"));
  mime_type_map.insert(std::pair<std::string, std::string>("image/svg+xml", "SVG vector image"));
  mime_type_map.insert(std::pair<std::string, std::string>("image/tiff", "Tag Image File Format"));

  mime_type_map.insert(std::pair<std::string, std::string>("text/html", "HTML"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/xml", "XML"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/css", "CSS"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/plain", "Plain Text"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/javascript", "Script resource (Javascript)"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/cmd", "Commands"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/csv", "Comma-separated values"));
  mime_type_map.insert(std::pair<std::string, std::string>("text/vcard", "vCard (contact information)"));

  mime_type_map.insert(std::pair<std::string, std::string>("audio/basic", "?-law audio at 8 kHz, 1 channel"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/L24", 
							   "24bit Linear PCM audio at 8?48 kHz, 1-N channels"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/mp4", "MP4 audio"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/mpeg", "MP3 or other MPEG audio"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/ogg", 
							   "Ogg Vorbis, Speex, Flac and other audio"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/vorbis", "Vorbis encoded audio"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/vnd.rn-realaudio", "RealAudio"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/vnd.wave", "WAV audio"));
  mime_type_map.insert(std::pair<std::string, std::string>("audio/webm", "WebM open media format"));

  mime_type_map.insert(std::pair<std::string, std::string>("multipart/mixed", "MIME Email"));
  mime_type_map.insert(std::pair<std::string, std::string>("multipart/alternative", "MIME Email"));
  mime_type_map.insert(std::pair<std::string, std::string>("multipart/related", "MIME Email"));
  mime_type_map.insert(std::pair<std::string, std::string>("multipart/form-data", "MIME Webform"));
  mime_type_map.insert(std::pair<std::string, std::string>("multipart/signed", "Defined in RFC 1847"));
  mime_type_map.insert(std::pair<std::string, std::string>("multipart/encrypted", "Defined in RFC 1847"));

  mime_type_map.insert(std::pair<std::string, std::string>("video/mpeg", "MPEG-1 video with multiplexed audio"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/mp4", "MP4 video"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/ogg", 
							   "Ogg Theora or other video (with audio)"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/quicktime", "QuickTime video"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/webm", 
							   "WebM Matroska-based open media format"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/x-matroska", "Matroska open media format"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/x-ms-wmv", "Windows Media Video"));
  mime_type_map.insert(std::pair<std::string, std::string>("video/x-flv</coda>", "Flash video (FLV files)"));
}
