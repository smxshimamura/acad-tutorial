// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Commands.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the Commands type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Application = Autodesk.AutoCAD.ApplicationServices.Core.Application;

[assembly: CommandClass(typeof(Lesson7.Commands))]
[assembly: ExtensionApplication(typeof(Lesson7.PluginExtension))]

namespace Lesson7
{
    /// <summary>
    /// The commands.
    /// </summary>
    public class Commands
    {
        /// <summary>
        /// Execute AddDBEvent command.
        /// </summary>
        [CommandMethod("Tutorials", "AddDBEvent", "AddDBEventId", CommandFlags.Modal)]
        public void AddDBEvent()
        {
            var database = HostApplicationServices.WorkingDatabase;
            database.ObjectAppended += this.OnObjectAppended;
            database.ObjectErased += this.OnObjectErased;
        }

        /// <summary>
        /// Execute RemDBEvent command.
        /// </summary>
        [CommandMethod("Tutorials", "RemDBEvent", "RemDBEventId", CommandFlags.Modal)]
        public void RemDBEvent()
        {
            var database = HostApplicationServices.WorkingDatabase;
            database.ObjectAppended -= this.OnObjectAppended;
            database.ObjectErased -= this.OnObjectErased;
        }

        private static void WriteToPrompt(DBObject obj, string text)
        {
            if (obj is Entity entity)
            {
                Application.DocumentManager.MdiActiveDocument.Editor.WriteMessage(entity.GetType().Name + text + Environment.NewLine);
            }
        }

        private void OnObjectAppended(object sender, ObjectEventArgs e) => WriteToPrompt(e.DBObject, " is appended.");

        private void OnObjectErased(object sender, ObjectErasedEventArgs e) => WriteToPrompt(e.DBObject, " is erased.");
    }
}
