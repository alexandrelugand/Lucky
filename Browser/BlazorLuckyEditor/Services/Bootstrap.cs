using System.Runtime.InteropServices;
using BlazorLuckyEditor.Interfaces.Services;

namespace BlazorLuckyEditor.Services
{
	public class Bootstrap : IBootstrap
	{
		[DllImport("LuckyEditor", EntryPoint = "Bootstrap_Init")]
		public static extern void Bootstrap_Init();

		public void Init() => Bootstrap_Init();
	}
}
