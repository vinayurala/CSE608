from subprocess import check_output
import sys
import time

def run_chrome(file_name):
	start_cmd_line = "adb.exe shell am start -n org.chromium.content_shell_apk/.ContentShellActivity -d "
	stop_cmd_line = "adb.exe shell am force-stop org.chromium.content_shell_apk"
	with open(file_name) as f:
		url_list = f.readlines()
	
	for url in url_list:
		print ("Starting chrome to browse " + str(url))
		cmd = start_cmd_line + url
		check_output(cmd, shell=True)
		target_time = time.clock() + 60
		while time.clock() < target_time:
			pass
		print ("Done with URL. Loading next URL")
		check_output(stop_cmd_line, shell=True)
		target_time = time.clock() + 5
		while time.clock() < target_time:
			pass
	
if __name__ == "__main__":
	if (len(sys.argv) == 1) :
		print ("Usage run_script.py <URL_List_File>")
		sys.exit(0)
	
	file_name = sys.argv[1]
	print ("Will crawl all the URLs specified in file: " + file_name)
	run_chrome(file_name)