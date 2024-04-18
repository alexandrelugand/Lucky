using System.Reflection;
using System.Runtime.InteropServices.JavaScript;
using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using BlazorLuckyEditor.Interfaces.Services;
using Lucky;
using Microsoft.JSInterop;

namespace BlazorLuckyEditor.Services
{
	[SupportedOSPlatform("browser")]
	public partial class ScriptingRuntime : IScriptingRuntime
	{
		private static IJSRuntime _jsRuntime = default!;
		private static readonly Dictionary<string, Assembly> Assemblies = new(StringComparer.InvariantCultureIgnoreCase);
		private static readonly Scripting Scripting = new();

		public ScriptingRuntime
		(
			IJSRuntime jsRuntime
		)
		{
			_jsRuntime = jsRuntime;
		}

		public async Task Init()
		{
			await _jsRuntime.InvokeVoidAsync("ScriptingRuntime.init");
		}

		[JSExport]
		public static void LoadLibrary(string assemblyName, byte[] data)
		{
			try
			{
				if (!Assemblies.TryGetValue(assemblyName, out var asm))
				{
					Scripting.Info($"Loading '{assemblyName}' library...");
					asm = AppDomain.CurrentDomain.Load(rawAssembly: data);
					Assemblies.Add(assemblyName, asm);
				}
			}
			catch (Exception ex)
			{
				Scripting.Error($"Failed to load '{assemblyName}' library: {ex}");
			}
		}

		[JSExport]
		public static IntPtr CreateInstance(string assemblyName, string className)
		{
			try
			{
				if (Assemblies.TryGetValue(assemblyName, out var asm))
				{
					var objType = asm.GetTypes().FirstOrDefault(t => t.Name == className);
					if (objType != null)
					{
						var instance = Activator.CreateInstance(objType);
						var gch = GCHandle.Alloc(instance, GCHandleType.Pinned);
						return GCHandle.ToIntPtr(gch);
					}
					Scripting.Error($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No class found");
				}
				else
					Scripting.Error($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No assembly found");
			}
			catch (Exception ex)
			{
				Scripting.Error($"Failed to create instance of '{className}' class (assembly: {assemblyName}): {ex}");
			}

			return IntPtr.Zero;
		}

		[JSExport]
		public static void Invoke(string assemblyName, string className, string method, [JSMarshalAs<JSType.BigInt>] long instance, [JSMarshalAs<JSType.Array<JSType.Any>>] object[] args)
		{
			try
			{
				if (Assemblies.TryGetValue(assemblyName, out var asm))
				{
					var objType = asm.GetTypes().FirstOrDefault(t => t.Name == className);
					if (objType != null)
					{
						var mi = objType.GetMethod(method, BindingFlags.Public | BindingFlags.Instance);
						if (mi != null)
						{
							var gch = GCHandle.FromIntPtr((IntPtr)instance);
							var obj = gch.Target;
							if (obj != null)
								mi.Invoke(gch.Target, args);
						}
						else
							Scripting.Error($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No method found");
					}
				}
				else
					Scripting.Error($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No assembly found");
			}
			catch (Exception ex)
			{
				Scripting.Error($"Failed to invoke '{method}' method of '{className}' class (assembly: {assemblyName}): {ex}");
			}
		}
	}
}
