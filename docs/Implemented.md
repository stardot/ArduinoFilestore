# Functions implemented by the Arduino Filestore

Here is an overview of which NetFS operations are currently working

| Status | Number | Description | Notes |
|--------|--------|-------------|-------|
| | 0 |	Decode command line | |
| |  |	*Access object_spec [attributes]| |
|:heavy_check_mark: |  |	*Bye| |
| :heavy_check_mark:|  |	*CDir directory| |
| :heavy_check_mark:|  |	*Delete object| |
| :heavy_check_mark:|  |	*Dir [directory]| |
|:heavy_check_mark: |  |	*I am user_name [password]| Now works correctly without password, See issue #33 |
| :heavy_check_mark: | |	*Info object_spec| |
|:heavy_check_mark: |  |	*Lib [directory]| |
|:heavy_check_mark: |  |	*Logon user_name [password]| |
|:heavy_check_mark: |  |	*Pass old_password new_password| |
| :heavy_check_mark:|  |	*Rename object new_name| |
| :heavy_check_mark:|  |	*SDisc [:]disc_spec| |
| :heavy_check_mark: |	| *FSShutdown| |
| :heavy_check_mark: |	| *FSReboot| |
| |  |	*Logoff user_name\|user_number| |
| :heavy_check_mark:|  |	*NewUser user_name| |
| :heavy_check_mark:|  |	*Priv user_name [new_privilege]| |
| :heavy_check_mark:|  |	*RemUser user_name| |
| :heavy_check_mark:|  |	*SetURD user_name directory| |
| :heavy_check_mark:|  |	*FSStatus| |
| :heavy_check_mark:|  |	*FSConfigure| |
| :heavy_check_mark: |1|	Save file| |
| :heavy_check_mark: |2|	Load file| |
| :heavy_check_mark: |3|	Examine||
| |4|	Catalogue header| |
|:heavy_check_mark: |5|	Load as| |
|:heavy_check_mark: |6|	Open object| |
|:heavy_check_mark: |7|	Close object| |
|:heavy_check_mark: |8|	Get byte| |
|:heavy_check_mark: |9|	Put byte| |
|:heavy_check_mark: |10|	Get bytes| |
|:heavy_check_mark: |11|	Put bytes| |
|:heavy_check_mark: |12|	Read random access arguments| |
|:heavy_check_mark: |13|	Set random access arguments| |
|:heavy_check_mark: |14|	Read disc information| |
|:heavy_check_mark: |15|	Read current users' information| |
|:heavy_check_mark: |16|	Read file server date and time| |
|:heavy_check_mark: |17|	Read 'End-of-file' status| |
|:heavy_check_mark: |18|	Read object information| |
|:heavy_check_mark: |19|	Set object attributes| |
|:heavy_check_mark: |20|	Delete object| |
|:heavy_check_mark: |21|	Read user environment| |
|:heavy_check_mark: |22|	Set user boot option| |
|:heavy_check_mark: |23|	Log off| |
| |24|	Read user's information| |
|:heavy_check_mark: |25|	Read file server version number| |
|:heavy_check_mark: |26|	Read disc free space information| |
|:heavy_check_mark: |27|	Create directory, specifying size| |
| |28|	Set file server date and time| Probably not required, as fetched from NTP daily|
|:heavy_check_mark: |29|	Create file| |
|:heavy_check_mark: |30|	Read user free space| Actually just returns disk space, as quotas not implemented|
| |31|	Set user free space| |
|:heavy_check_mark: |32|	Read client UserId| |
| |33|	Read current users' info (extended)| |
| |34|Read user's information (extended)| |
| |35| *Reserved*	| |
| |36| 	Manager interface| |
| |37| *Reserved*| |

# Implementation details

Directories should be correctly identified as Owner and Public based on the user root directory in their profile, and public /
private attributes are enforced based on the complete path - not the usage of &. This is different to Acorn servers, in that
your private permissions are preserved if you traverse back to your URD from the server root.

It's undefined how any existing files on the SD card will be handled if they break the NetFS naming rules. Also these rules
are not checked in several places, so it is possible to create an illegal NetFS name with \*CDIR or \*RENAME, or copying a file
to NetFS from another FS which has different rules. But they can be \*DELETED if it's not possible to get them into a format
that is accessible.
