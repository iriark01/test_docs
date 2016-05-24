---
title: Using Factory Tool
---

High Level Architecture
=======================

Overview
--------

Factory tool serve as RPRS (Root Provisioning Server) for manufacturing
line. Factory tool provides:

1.  Cicv creation

2.  ROT Enrollment message

3.  Factory key based provisioning messages.

4.  Factory key based delegation messages (currently not supported).

5.  etc.

Packaging
---------

The factory tool is provided as a zip file which contains the following
directories:

-   cli: Contains CLI executable

-   conf: Provided in order to allow optional configurations

-   resources: External resources to be used by the server/cli.

-   server: The server jar & other server related files

-   work: Working directory of the cli/server

-   logs: Will be used for server/cli logs

-   injectionToolReference:\
    > Contains a reference implementation for server side injection tool

**NOTE**

> Different packages are provided for different operating systems.
>
> Currently only Windows / Linux 64-bit versions are provided.

1.  

Flow
----

![](media/image1.png){width="5.7625in" height="5.404166666666667in"}

High Level Design
=================

Tool Components
---------------

-   -   Injection tool reference implementation:

    -   The injection tool implementation is factory specific

    -   The reference injection tool is a python library provided
        supports the flow of devices with self generated key ability,
        connected by TCP connection.

-   Factory tool command line interface (CLI)

    -   The factory tool CLI acts as a mediator between the injection
        tool and server components.

    -   It allows creation of multiple messages by executing a batch
        of commands.

    -   The syntax of the batch is defined later in this document.

-   -   Server (server/prs-factory-springboot-boot.jar)

Lock mechanism
--------------

#### In order to prevent parallel execution the server uses some locking mechanism. {#in-order-to-prevent-parallel-execution-the-server-uses-some-locking-mechanism. .ListParagraph}

#### When server start up it check for the existence of factory.lock file under the work directory in the extracted archive. {#when-server-start-up-it-check-for-the-existence-of-factory.lock-file-under-the-work-directory-in-the-extracted-archive. .ListParagraph}

#### If the file exist the server exits with a corresponding error in log. {#if-the-file-exist-the-server-exits-with-a-corresponding-error-in-log. .ListParagraph}

#### If the file doesn't exist the server generates one and write into it a json structure with the following field: {#if-the-file-doesnt-exist-the-server-generates-one-and-write-into-it-a-json-structure-with-the-following-field .ListParagraph}

#### pid – the pid of the server process

#### On graceful shutdown the server remove that file. {#on-graceful-shutdown-the-server-remove-that-file. .ListParagraph}

#### Note: in case the server was terminated abruptly, the file will not be removed and the lock file needs to be removed manually. {#note-in-case-the-server-was-terminated-abruptly-the-file-will-not-be-removed-and-the-lock-file-needs-to-be-removed-manually. .ListParagraph}

Port Allocation
---------------

1.  When using factory tool CLI, the CLI will run a factory tool server.
    This is used in order to minimize java ramp-up time.

2.  This server will accept configuration from CLI on a randomly
    allocated port.

3.  To overwrite random port by a specific port:

    a.  Go to 'conf' directory under factory-tool
        installation directory.

    b.  Edit file 'application-extension.properties':

        i.  Change 'server.port' value to requested port number.

4.  A file is then created with process ID (PID) of the server

    a.  This file will be used by factory CLI to:

        i.  Avoid running multiple server instances

        ii. Understand which http port shall be used

        iii. Terminate server when requested by user

Build Process
=============

The build of the factory tool is made as part of the PRS gradle build.

The build shall be ran on:

-   A Linux machine to allow CLI executable that can run on Linux

-   A windows machine to allow CLI executable that can run on windows

Installation Process
====================

Prerequisites
-------------

### HW Requirements

N/A

1.  

### SW Requirements

Following SW shall be installed on machine prior to installation:

-   Java 8

1.  

Factory Tool Installation
-------------------------

### Package installation

-   Unzip package file (factoryTool.zip) to desired destination. Folder
    structure is described in 'Packaging' section above.

-   See 'First time configuration' section below and provide the
    relevant configurations / resources prior to using the CLI.

-   -   -   -   -   -   -   

### First time configuration

#### CPRS:

-   Shall be located under 'resources' directory.

    -   -   

#### application-extension.properties

-   Optional configuration file which may be located under
    'conf' directory.

-   Contains specific properties to override server configuration, e.g.
    port.

The factory-specific properties that can be overridden:

server.port To overwrite random port by a specific port

server.inactivity.timeout

> Number of minutes allowed for inactivity of the
>
> Server (not getting and not processing requests). After
>
> inactivity timeout the server will shut down (Default: 10 minutes).
> The server is checked for inactivity every 5 seconds.

factory.cprsFileLocation

> The location of the Cprs file in the file system

#### Metadata file of server

-   RSthe prsFactory\_metadata.json

-   File is l

-   RS

-   This information is provided to allow registration of factory tool
    at the PRS online as a trusted server.

Using Injection Tool
====================

-   The Injection tool is factory specific , as for each factory there
    may be:

    -   Different ways to reveal device information 

    -   Different ways to connect to device

    -   Number of concurrent connected devices may vary

    -   Extra information/logic to inject to device other than trusted
        data

-   -   The reference implementation provided by ARM is implemented
    in python.

-   The reference implementation provided by ARM supports enrollment by
    public keys (SGKM). This flow includes the following operations:

    -   Connecting to the device.

    -   Receiving the public keys from the device.

    -   Creating the SPV messages based on the public keys, according to
        the commands in the sample file 'sgkm\_sample.txt'.

    -   Sending the SPV messages to the device.

-   The source code can be found under the injectiontool folder in the
    injectionToolReference directory.

-   To run the injection tool:

    -   Go to the injectionToolReference directory

    -   Usage:

python -m injectiontool &lt;Device IP&gt; \[options\]

python -m injectiontool --help

Options:

--help Outputs help information

--port The device port (default: 7777)

--factory\_tool The path to the factory tool executable from extracted
zip (default: cli/factory\_tool)

--input\_file The path to the input file that contains the commands for
the factory tool (default: injectiontool/sgkm\_sample.txt)

-   -   

<!-- -->

-   

Factory Tool Command Line (CLI)
===============================

The command line interface provided by the factory tool is intended to
be used by the injection tool.

CLI Usage
---------

The CLI supports 2 methods:

### Create Message

The 'create\_message' command is intended for generating the messages
intended for device.

#### Syntax

factory\_tool \[options\] create\_message &lt;batch\_file\_name&gt;

Options:

--verbose -v Increase verbosity of messages.\
Default verbosity level is error. This flag can be used more than once,
to provide more information in the factory tool log.

#### Parameters

batch\_file\_name: Path to the file containing list of commands to
create relevant messages for device. File syntax is described below.

#### Command output

When successful:

There are 2 options for the output file format: Zip and Raw. The format
is specified in the "Device" command in the batch file (default if not
specified: Zip).

-   In the Zip format:

1)  A zip file will be produced, containing the messages intended for
    the device.

-   In the Raw format:

    1\) A binary file will be produced, containing the messages intended for
    the device in TLV format (the messages are concatenated in the payload).

2\) Text in JSON format will be written to standard output, specifying
the full path of this file. The format will look as following:

> {
>
> "outputFileName": "/some-path/file-name.zip"
>
> }

3\) Command will exit with success (return 0).

When failed:

1)  Error log will be redirected to system standard error (stderr).

2)  Command will exit with failure (return 1).

### Terminate server

The 'terminate\_server' command is intended for stopping the RPRS server
which was used by the CLI until the point of termination.

#### Syntax

factory\_tool \[options\] terminate\_server

Options:

--verbose -v Increase verbosity of messages.

#### Command output

1)  In case 'application\_metadata.json' file does not exist under
    'work' dir, command will print to output:

    **Server port file not found**

2)  In case process was not found, command will print to output:

    **No process with pid &lt;pid&gt;**

3)  In case server was terminated, command will print to output:

    **process &lt;pid&gt; terminated with exit code &lt;exit\_code&gt;**

### Batch File Syntax

#### General 

Following are general rules for batch file processing:

1)  The order of commands is important and treated accordingly.

2)  User can define parameters and use their value later in the batch.

3)  The 'device' command must be the first command in the batch.

4)  An 'enroll' command must exist in batch.

5)  Other commands are optional.

6)  The type of some command parameters is byte array.

    a.  The common case for using such parameter is for encryption keys.

    b.  The value for such parameters can be provided by either:

        i.  A named parameter, loaded from file

        ii. Specifying the string representation of base64 encoding of
            the bytes.

    c.  The following sets of commands are equal (assuming file contents
        are the same as text):

> load class-key ../input/some-class.key
>
> set\_class\_key \$class-key
>
> set class-key q83veJASNFZ4kBI0VniQEg==
>
> set\_class\_key \$class-key
>
> set\_class\_key q83veJASNFZ4kBI0VniQEg==

1)  Empty lines are ignored.

2)  Lines starting with hash sign (\#) are considered as comments,
    thus ignored.

#### Commands

##### Device

The 'device' command shall be the first command in any batch file.

###### Syntax

Device &lt;output\_file\_name&gt; --file-format=&lt;file-format&gt;
\[options\]

Options:

-- file-format =\[zip|raw\]\
Output file format to be used for device (Default: zip)

##### **Handling Named Parameters**

The following commands allow the user set a value for a named parameter
(e.g. 'some-param' and use it later on the script with '\$' prefix (e.g.
'\$some-param').

###### Set

Set a parameter value as is:

Set &lt;param\_name&gt; &lt;param\_value&gt;

###### Load

Load a parameter value from a file with given path. Most useful for
parameters which require byte array values, such as public keys.

Load &lt;param\_name&gt; &lt;file\_name&gt;

##### Set Class Key

The 'set\_class\_key' command may exist anywhere in the batch or may be
missing.

The command specifies the symmetric key to be used by the device for
decrypting messages received from a PRS (also referred as class-key)
after factory enrollment procedure is complete.

###### Syntax

Set\_class\_key &lt;class\_key&gt;

The class\_key parameter is a byte array representation of the symmetric
key.

When this command is not present in batch, device class-key will be
'none'.

In case the command exists more than once in batch, last key will be
used.

##### Enroll

A single 'enroll' command must exist in the batch, and precede any
provision/delegate commands.

The command allows device first acknowledgement of the RPRS.

It also allows the device decrypt further messages received from the
factory PRS.

There are several types of enrollments used according to factory setup:

###### Syntax

Enroll --pub-key-report=&lt;key\_report&gt; \[options\]

Options:

--pub-key-report=&lt;key\_report&gt; A byte array containing public key
report of a device

--allow=&lt;asset\_name&gt; Asset name to be included in device asset
white list, meaning device may be able to use such asset.\
This switch may be repeated several times to allow multiple values.

--disallow=&lt;asset\_name&gt; Asset name to be included in device asset
black list, meaning device cannot use such asset.\
This switch may be repeated several times to allow multiple values.

-   The 'allow' and 'disallow' switches are strings specifying an asset
    name to be included in device asset white and black
    lists accordingly.

    -   These switches can be repeated with different asset names.

    -   Asset name can be suffixed with asterisk '\*'.

###### 

-   -   

###### 

-   

###### 

-   -   

###### 

-   

##### Provision

Provisioning allows the device work with the manufacturer's assets
(files) in an encrypted way, so that others will not be able to use
these assets.

Multiple 'provision' commands may exist in the batch; all must follow a
previous 'enroll' command.

provision --asset-name=&lt;asset\_name&gt;
--asset-data=&lt;asset\_data&gt; \[options\]

Options:

--asset-name Defines the asset name to be used (required)

--asset-data Byte array containing the file contents to be encrypted
(required)

--access-method=\[unlimited|password-auth|specific-ta-auth|all-tas-auth\]\
The asset access method type (Default: unlimited)

--persistency-mode=\[read-only|transient|factory|removable|volatile\]\
The asset persistency mode (Default: read-only(

--passphrase Byte array containing password to protect the asset
payload, passphrase must be provided if access-method is password-auth.
(Default: null)

--password-iteration-count Number of iterations for generating a secure
key based on the passphrase - a larger number creates a more secure key,
but consumes more CPU cycles on the device, it must be ranged between
100 - 10,000.\
Must be provided in case passphrase is provided (Default: 10,000(

--rollback-protected=\[true|false\]\
Enable rollback protection for this asset (Default: false)

--trustlet-id A trustlet-id is specified as a combination of OS type and
TA identifier separated by the ":" sign.\
The OS type may be up to 32 chars long and contain digits, letters and
underscores (i.e.: T\_BASE).\
The TA identifier may be up to 128 hexa-decimal chars long preceded by
"0x" (i.e.: 0x11AA22BB33CC44FF).\
Given the above a trustlet-id is a combination of OS type and TA
identifier hence its final form would be: T\_BASE:0x11AA22BB33CC44FF.\
Every asset may have multiple trustlet-ids, thus switch may be repeated
several times to allow multiple values.\
Mandatory when access-method is specific-ta-auth.

--secure-asset=\[true|false\]\
Determine if Asset is secure (readable) or not (Default: true)

.

Examples
--------

### Device enrollment only, no white/black lists

Device ../work/test-device\#\#\#.zip

Load key-report ../work/pub-key-report

Enroll --key-report=\$pub-key-report

### Device enrollment with single provisioning

Device ../work/test-device\#\#\#.zip

Load key-report ../work/pub-key-report

Enroll --key-report=\$pub-key-report

Load asset1 /assets/asset1

Provision --asset-name=asset1 --asset-data=\$asset1

Load asset2 /assets/asset2

Provision --asset-name=asset2 --asset-data=\$asset2
--access-method=password-auth\
--passphrase=q83veJASNFZ4kBI0VniQEg== --rollback-protected=true

### Device enrollment only, with asset white list

Device sample.zip

Load key-report ../work/pub-key-report

Enroll --key-report=\$pub-key-report --allow=arm\* --allow=lg\*

Troubleshooting
===============

Log File
--------

Log files can be found under 'logs' directory of extracted folder.

CLI Errors
----------

TBD
