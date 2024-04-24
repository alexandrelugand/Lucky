using Lucky.Maths;

namespace Lucky
{
	public class Main : BaseScript
	{
		public Main()
		{
			Scripting.Info("Main constructor!");
		}

		public void PrintMessage()
		{
			Scripting.Info("Hello world from C# Scripting!");
			var vector2 = new Vec2 { x = 1.0f, y = 2.0f };
			var vector3 = new Vec3 { x = 1.0f, y = 2.0f, z = 3.0f };
			var vector4 = new Vec4 { x = 1.0f, y = 2.0f, z = 3.0f, w = 4.0f };
			Scripting.LogVec2(ref vector2);
			Scripting.LogVec3(ref vector3);
			Scripting.LogVec4(ref vector4);
		}

		public override void OnUpdate(string method, float ts)
		{
			Scripting.Info($"{method}: {ts}");
		}
	}
}
