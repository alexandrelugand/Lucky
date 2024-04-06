var Browser = {
	init() {
		function pointerLockChange() {
			Browser.pointerLock = document['pointerLockElement'] === Module['canvas'];
		}

		Module["canvas"] = document.getElementById("canvas");
		var canvas = Module['canvas'];
		if (canvas) {

			canvas.requestPointerLock = canvas['requestPointerLock'] || (() => { });
			canvas.exitPointerLock = document['exitPointerLock'] || (() => { });
			canvas.exitPointerLock = canvas.exitPointerLock.bind(document);

			document.addEventListener('pointerlockchange', pointerLockChange, false);
			if (Module['elementPointerLock']) {
				canvas.addEventListener("click", (ev) => {
					if (!Browser.pointerLock && Module['canvas'].requestPointerLock) {
						Module['canvas'].requestPointerLock();
						ev.preventDefault();
					}
				}, false);
			}
		}
	}
};
