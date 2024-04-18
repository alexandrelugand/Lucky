using System;

namespace Lucky
{
	public class Main
	{
		public static Scripting Scripting = new Scripting();

		public Main()
		{
			Scripting.Info("Main constructor!");
		}

		public void PrintMessage()
		{
			Scripting.Info("Hello world from C# Scripting!");
		}

		public void OnUpdate(double ts)
		{
			Scripting.Info($"OnUpdate: {ts}");
		}
	}
}
