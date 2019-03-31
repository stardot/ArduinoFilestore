# Filesystem Layout

By default the software will create the following folder in the root directory of the SD card of not present

- export
- meta
- profile

## export folder
This holds the files that have been stored on the fileserver. Case of files is preserved in their creation but like a real 
Filestore any other fileoperations are case agnostic. 

The default name of the folder is export, however this can be changed in the server configuration profile.

## meta folder
As each file is written to the Filestore via the network, a corresponding meta file is produced in this folder. If a file in the
export folder does not have a meta file for some reason (perhaps it already existed on the card) then a default meta file is 
created of filetype &FFD (Data) and the date stamp is read from the FAT filesystem. 

Folders also have metafiles created, although they use the filename $ on the SD card - as that is not a legal FileCore filename,
and so cannot cause a conflict.

The format of the metafile is:
- Bytes 1-4 Load address
- Bytes 5-8 Exec Address
- Byte 9 Attributes

The default name of the folder is meta, however this can be changed in the server configuration profile.

## profile folder
Each Filestore user has a profile created in this folder. The contents are much the same as the Filestore passwords file, but each user has a different file in this folder, which is named by their username.

Layout of the user file is:
- Byte 1 - Privilege
- Byte 2 - Boot options
- Byte 3 - Account enabled
- Byte 4 onwards - Password
- Byte 27 onwards - Reserved (Will be used for quota)
- Byte 35 onwards - User root directory

The default name of the folder is profile, however this can be changed in the server configuration profile.

## config folder
The statup configuration of the server is contained in a number of key / value pairs. Each configuration value is contained in a file in this folder named after the key. All values are stored as strings, and the FS code will cast between datatypes as required. 

You can examine a configuration value with *FSSTATUS _key_, and the values can be changed with *FSCONFIGURE _key_ _value_ from a station logged on with system rights.

The folder name is hard coded as config, and requires a code change to modify.
