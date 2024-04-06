using System.Runtime.Versioning;
using BlazorLuckyEditor.Interfaces.Services;
using BlazorLuckyEditor.Services;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using Microsoft.JSInterop;

namespace BlazorLuckyEditor
{
	[SupportedOSPlatform("browser")]
	public class Program
	{
		public static async Task Main(string[] args)
		{
			var builder = WebAssemblyHostBuilder.CreateDefault(args);
			builder.RootComponents.Add<App>("#app");
			builder.RootComponents.Add<HeadOutlet>("head::after");
			builder.Services.AddScoped(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) });
			builder.Services.AddSingleton(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) });
			builder.Services.AddSingleton<ILuckyEditorManager, LuckyEditorManager>();
			builder.Services.AddSingleton<IScriptingRuntime, ScriptingRuntime>();

			var host = builder.Build();
			var luckyEditorManager = host.Services.GetService<ILuckyEditorManager>();
			if (luckyEditorManager != null)
				await luckyEditorManager.Init();
			await host.RunAsync();
		}
	}
}
