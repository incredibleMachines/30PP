ControllerApp/Media Manager Application
--------------------------------------------

### Requirements:
	http://nodejs.org/
 	http://brew.sh/
 	http://www.mongodb.org/

### Instructions:
Ensure you have node.js homebrew and mongodb installed.
_*Note Recommended to install Mongodb via Homebrew_

Make sure that mongod is running, open terminal and type:
```bash
$ mongod
```
Running the application, open terminal and type:
```bash
$ cd /path/to/30_PP/ControllerApp

$ npm install

$ node app.js
```
View system by opening a browser and directing it to __[http://localhost:3000](http://localhost:3000)__

#### Reload Mongo
A method for restoring your database from a dump on github.


##### Manually Removing DB Items

Open Terminal and ensure that Mongod is running *See Above*
```javascript
$ mongo 30_PP

> db.dropDatabase()

```
_In another Tab_

```bash

$ cd /path/to/30PP/ControllerApp

$ mongorestore

```
_Optionally remove all items from render queue_

```bash
#assuming you are still in mongo 30_PP
>db.clips.update({render:true},{render:false},true,true)

```
##### Open hidden assets folder 
Open Terminal and ensure you are in 30_PP/ControllerApp/ 
```javascript
$ open public/.assets

```
##### Kill process by PID
First, find the process PID using Activity Monitor.
In terminal, type  
```javascript
$ sudo kill -SIGTERM [PID]

```
