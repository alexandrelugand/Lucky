var FS = {
	instance: undefined,
	init() {
		var fs = Module.FS;
		console.log("Mounting IDBFS databases... ");
		var db = "/assets";
		fs.mkdir(db);
		fs.mount(fs.filesystems.IDBFS, {}, db);

		db = "/resources";
		fs.mkdir(db);
		fs.mount(fs.filesystems.IDBFS, {}, db);

		fs.syncfs(true, function (err) {
			if (err != undefined)
				console.log("Mount IDBFS databases failed. " + err);
		});
	},
	syncFs() {
		console.log("Synchronize file system...");
		Module.FS.syncfs(false, function (err) {
			if (err != undefined)
				console.log("Sync IDBFS database failed. " + err);
		});
	},
	dirExists(dirPath) {
		try {
			var lookup = Module.FS.lookupPath(dirPath);
			if (lookup.node !== undefined)
				return true;
		} catch {
			return false;
		}
	},
	createDirectory(dirPath) {
		if (!FS.dirExists(dirPath)) {
			console.log("Creating '" + dirPath + "' directory...");
			Module.FS.mkdir(dirPath);
		}
	},
	writeFile(filePath, content) {
		console.log("Writing '" + filePath + "' file...");
		Module.FS.writeFile(filePath, content);
	}
};
