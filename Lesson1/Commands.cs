// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Commands.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the Commands type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

using Autodesk.AutoCAD.Runtime;
using Application = Autodesk.AutoCAD.ApplicationServices.Core.Application;

[assembly: CommandClass(typeof(Lesson1.Commands))]
[assembly: ExtensionApplication(typeof(Lesson1.PluginExtension))]

namespace Lesson1
{
    /// <summary>
    ///  Provides any commands.
    /// </summary>
    public class Commands
    {
        /// <summary>
        /// Execute Hello command.
        /// </summary>
        [CommandMethod("Tutorials", "Hello", "HelloId", CommandFlags.Modal)]
        public void Hello()
        {
            var doc = Application.DocumentManager.MdiActiveDocument;
            if (doc != null)
            {
                doc.Editor.WriteMessage("Hello, World!" + Environment.NewLine);
            }
        }
    }
}
