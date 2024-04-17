using System;

namespace Lucky
{
	public class Main
	{
		public static LuckyApi Api = new LuckyApi();

		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main constructor!");
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello world from C# Scripting!");
			//using (var api = new LuckyApi())
			//{
			//	api.Log("Hello world from C# scripting to C++!");
			//}
			Api.Log("Hello world from C# scripting to C++!");
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
	}
}
