using System;
using System.IO;
using System.Reflection;

namespace Lucky
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main constructor!");
			AppDomain.CurrentDomain.AssemblyResolve += CurrentDomainOnAssemblyResolve;
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello world from C#!");
		}

		public void PrintInt(int value)
		{
			Console.WriteLine($"C# says: {value}");
		}

		public void PrintInts(int value1, int value2)
		{
			Console.WriteLine($"C# says: {value1} and {value2}");
		}

		public void PrintCustomMessage(string message)
		{
			Console.WriteLine($"C# says: {message}");
		}

		public void OnUpdate(double ts)
		{
			Console.WriteLine($"OnUpdate: {ts}");
		}

		private Assembly CurrentDomainOnAssemblyResolve(object sender, ResolveEventArgs args)
		{
			Console.WriteLine($"AssemblyResolve: {args}");
			var execPath = Path.GetDirectoryName(Assembly.GetEntryAssembly()?.Location);
			if (!string.IsNullOrEmpty(execPath))			{
				var name = args.Name;
				if (args.Name.IndexOf(',') != -1)
					name = args.Name.Substring(0, args.Name.IndexOf(','));
				var filename = Path.Combine(execPath, name);
				if (File.Exists(filename))
					return Assembly.LoadFrom(filename);
				if (File.Exists(filename + ".dll"))
					return Assembly.LoadFrom(filename + ".dll");
				if (File.Exists(filename + ".exe"))
					return Assembly.LoadFrom(filename + ".exe");
			}
			return null;
		}
	}
}
