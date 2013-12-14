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

instrumenter::instrumenter()
{
  this->populate_mime_type_map();
  this->log_file_dir = "/sdcard/";
  this->log_file_name_prefix = "Instrumented_data";
  this->log_file_name_extension = ".xml";
}

instrumenter::instrumenter(string log_file_dir,
			   string log_file_name_prefix, 
			   string log_file_name_extension)
{
  this->populate_mime_type_map();
  
  this->log_file_dir = log_file_dir;
  if (log_file_dir[log_file_dir.length() - 1] != '/')
    this->log_file_dir += "/";

  this->log_file_name_prefix = log_file_name_prefix;
  this->log_file_name_extension = log_file_name_extension;
}

instrumenter::~instrumenter()
{
}

bool instrumenter::log_instrument_data(string log_level, string tag, string log_msg,
				       pid_t pid, string curr_time) 
{

  if (log_level.empty() || tag.empty() || log_msg.empty())
    return false;
  
  if (pid == -1)
    pid = getpid();

  string file_name = this->log_file_dir 
    + this->log_file_name_prefix + "_"
    + this->stringify(pid)
    + this->log_file_name_extension;

  ifstream in_file(file_name.c_str(), ios::in);
  ofstream out_file;
  
  if (curr_time == "")
    curr_time = get_time();

  // if file exists, just append; else create a new one
  if (in_file)
    out_file.open(file_name.c_str(), ios::out | ios::app);
  else
    out_file.open(file_name.c_str(), ios::out);

  string buffer = "<Event>\n\t";
  buffer += build_xml_element("Time", curr_time, false);
  buffer += build_xml_element("PID", this->stringify(pid), false);
  buffer += build_xml_element("Level", log_level, false);
  buffer += build_xml_element("Tag", tag, false);
  buffer += build_xml_element("Message", log_msg, true);
  buffer += "</Event>\n";
  out_file << buffer;

  buffer.clear();
  out_file.close();
  in_file.close();

  return true;
}

string instrumenter::build_xml_element(string key, string val, 
				       bool end_with_new_line)
{
  if (end_with_new_line)
    return "<" + key + "> " + val + " </" + key + ">\n";
  else
    return "<" + key + "> " + val + " </" + key + ">\n\t";
}

string instrumenter::get_time()
{
  time_t now = time(0);
  struct tm tm_struct;
  char buf[80];
  tm_struct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tm_struct);
  return buf;
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
