using System.Reflection;
using System.Runtime.InteropServices.JavaScript;
using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using BlazorLuckyEditor.Interfaces.Services;
using Microsoft.JSInterop;

namespace BlazorLuckyEditor.Services
{
	[SupportedOSPlatform("browser")]
	public partial class ScriptingRuntime : IScriptingRuntime
	{
		private static IJSRuntime _jsRuntime = default!;
		private static readonly Dictionary<string, Assembly> Assemblies = new(StringComparer.InvariantCultureIgnoreCase);

		public ScriptingRuntime
		(
			IJSRuntime jsRuntime
		)
		{
			_jsRuntime = jsRuntime;
		}

		public async Task Init()
		{
			Console.WriteLine("Initialize scripting...");
			await _jsRuntime.InvokeVoidAsync("Scripting.init");
		}

		[JSExport]
		public static void PrintMessage()
		{
			Console.WriteLine("Hello world from C#!");
			_jsRuntime.InvokeVoidAsync("window.testfunction", 10);
		}

		[JSExport]
		public static void PrintInt(int value)
		{
			Console.WriteLine($"C# says: {value}");
		}

		[JSExport]
		public static void PrintInts(int value1, int value2)
		{
			Console.WriteLine($"C# says: {value1} and {value2}");
		}

		[JSExport]
		public static void PrintCustomMessage(string message)
		{
			Console.WriteLine($"C# says: {message}");
		}

		[JSExport]
		public static void LoadLibrary(string assemblyName, byte[] data)
		{
			try
			{
				if (!Assemblies.TryGetValue(assemblyName, out var asm))
				{
					Console.WriteLine($"Loading '{assemblyName}' library...");
					asm = AppDomain.CurrentDomain.Load(rawAssembly: data);
					Assemblies.Add(assemblyName, asm);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Failed to load '{assemblyName}' library: {ex}");
			}
		}

		[JSExport]
		public static IntPtr CreateInstance(string assemblyName, string className)
		{
			try
			{
				if (Assemblies.TryGetValue(assemblyName, out var asm))
				{
					Console.WriteLine($"Creating instance of '{className}' class (assembly: {assemblyName})...");
					var objType = asm.GetTypes().FirstOrDefault(t => t.Name == className);
					if (objType != null)
					{
						var instance = Activator.CreateInstance(objType);
						var gch = GCHandle.Alloc(instance, GCHandleType.Pinned);
						return GCHandle.ToIntPtr(gch);
					}
					Console.WriteLine($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No class found");
				}
				else
					Console.WriteLine($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No assembly found");
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Failed to create instance of '{className}' class (assembly: {assemblyName}): {ex}");
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
					Console.WriteLine($"Invoking '{method}' method (assemblyName: {assemblyName}, className: {className}, instance: {instance}, args: {string.Join(',', args)})...");
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
							Console.WriteLine($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No method found");
					}
				}
				else
					Console.WriteLine($"Failed to create instance of '{className}' class (assembly: {assemblyName}): No assembly found");
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Failed to invoke '{method}' method of '{className}' class (assembly: {assemblyName}): {ex}");
			}
		}
	}
}
