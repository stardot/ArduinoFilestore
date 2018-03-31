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
|:heavy_check_mark: |  |	*I am user_name [password]| Sometimes confuses FS number with userid|
| |  |	*Info object_spec| |
|:heavy_check_mark: |  |	*Lib [directory]| |
| |  |	*Logon user_name [password]| |
|:heavy_check_mark: |  |	*Pass old_password new_password| |
| :heavy_check_mark:|  |	*Rename object new_name| |
| :heavy_check_mark:|  |	*SDisc [:]disc_spec| |
| |  |	*FSShutdown| |
| |  |	*Logoff user_name\|user_number| |
| :heavy_check_mark:|  |	*NewUser user_name| |
| :heavy_check_mark:|  |	*Priv user_name [new_privilege]| |
| :heavy_check_mark:|  |	*RemUser user_name| |
| :heavy_check_mark:|  |	*SetURD user_name directory| |
|:heavy_check_mark: |1|	Save file| |
|:heavy_check_mark: |2|	Load file| |
|:heavy_check_mark: |3|	Examine| Does not support return format 1|
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
|:heavy_check_mark: |30|	Read user free space| |
| |31|	Set user free space| |
|:heavy_check_mark: |32|	Read client UserId| |
| |33|	Read current users' info (extended)| |
| |34|Read user's information (extended)| |
| |35| *Reserved*	| |
| |36| 	Manager interface| |
| |37| *Reserved*| |

# Implementation details


