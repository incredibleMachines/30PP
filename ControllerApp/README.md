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

#### Creating a backup of your Mongo database
Steps to duplicate your database and share it on git

##### Delete the dump folder and create a new one

```bash
$ cd /path/to/30PP/ControllerApp

$ rm -rf dump/

$ mongodump -db 30_PP
```

__Copy your .assets folder to Dropbox__

```bash
$ cd /path/to/30PP/ControllerApp

$ cp -r public/.assets path/to/your/Dropbox\ \(Incredible\ Machines\)/30PP/AE_Architecture/Database/assets
```

__Pull assets down from Dropbox __

```bash
$ cd /path/to/30PP/ControllerApp

$ cp -r ~/Dropbox\ \(Incredible\ Machines\)/30PP/AE_Architecture/Database/assets/ public/.assets/
```

#### Reload Mongo from git store
A method for restoring your database from a dump on github.


##### Manually Removing DB Items

Open Terminal and ensure that Mongod is running *See Above*
```bash
$ mongo 30_PP

> db.dropDatabase()
```
_In another Tab_

```bash
$ cd /path/to/30PP/ControllerApp

$ mongorestore
```
Optionally remove all items from render queue _Go back to the Mongo Terminal Tab_

```javascript
> db.clips.update({render:true},{$set:{render:false}},{multi:true})
```

##### Helpful Bash Commands

A few random but very helpful commands to manage the development process

##### Open hidden assets folder
Open Terminal and ensure you are in 30_PP/ControllerApp/
```bash
$ open public/.assets
```

##### Kill process by PID
First, find the process PID using Activity Monitor.
In terminal, type  
```bash
$ sudo kill -SIGTERM [PID]
```
