var ScriptingRuntime = {
	async init() {
		var exports = await Blazor.runtime.getAssemblyExports("BlazorLuckyEditor");
		window.Scripting = exports.BlazorLuckyEditor.Services.ScriptingRuntime;
	}
};
