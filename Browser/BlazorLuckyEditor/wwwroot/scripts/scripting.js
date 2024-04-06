var Scripting = {
	async init() {
		var exports = await Blazor.runtime.getAssemblyExports("BlazorLuckyEditor");
		window.ScriptRuntime = exports.BlazorLuckyEditor.Services.ScriptingRuntime;
	}
};
