using BlazorLuckyEditor.Interfaces.Services;
using Microsoft.JSInterop;

namespace BlazorLuckyEditor.Services
{
    public class LuckyEditorManager : ILuckyEditorManager
    {
	    private readonly IJSRuntime _jsRuntime;
	    private readonly IScriptingRuntime _scriptingRuntime;
	    private readonly HttpClient _httpClient;

	    public LuckyEditorManager
	    (
			IJSRuntime jsRuntime,
			IScriptingRuntime scriptingRuntime,
		    HttpClient httpClient
		)
	    {
		    _jsRuntime = jsRuntime;
		    _scriptingRuntime = scriptingRuntime;
		    _httpClient = httpClient;
	    }

	    public async Task Init()
	    {
		    await _jsRuntime.InvokeVoidAsync("FS.init");
			await DownloadFiles();
		    await _scriptingRuntime.Init();
	    }

	    private async Task DownloadFiles()
	    {
		    var fileSystemContent = await _httpClient.GetStringAsync("filesystem.txt");
		    foreach (var filePath in fileSystemContent.Split(new[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries))
		    {
				var ext = Path.GetExtension(filePath);
				if (string.IsNullOrEmpty(ext))
				{
					await _jsRuntime.InvokeVoidAsync("FS.createDirectory", filePath);
				}
				else
				{
					var dirPath = Path.GetDirectoryName(filePath);
					if (!string.IsNullOrEmpty(dirPath))
					{
						await _jsRuntime.InvokeVoidAsync("FS.createDirectory", dirPath);
						var fileContent = await _httpClient.GetByteArrayAsync(filePath);
						await _jsRuntime.InvokeVoidAsync("FS.writeFile", filePath, fileContent);
					}
				}
			}
		    await _jsRuntime.InvokeVoidAsync("FS.syncFs");
		}
    }
}
