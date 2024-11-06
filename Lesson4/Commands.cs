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
using Exception = System.Exception;

[assembly: CommandClass(typeof(Lesson4.Commands))]
[assembly: ExtensionApplication(typeof(Lesson4.PluginExtension))]

namespace Lesson4
{
    using System.Transactions;

    using Autodesk.AutoCAD.Colors;
    using Autodesk.AutoCAD.EditorInput;

    /// <summary>
    /// The commands.
    /// </summary>
    public class Commands
    {
        /// <summary>
        /// Execute ChangeColor command.
        /// </summary>
        [CommandMethod("Tutorials", "ChangeColor", "ChangeColorId", CommandFlags.Modal)]
        public void ChangeColor()
        {
            var doc = Application.DocumentManager.MdiActiveDocument;
            var database = HostApplicationServices.WorkingDatabase;
            var transaction = database.TransactionManager.StartTransaction();
            try
            {
                var entityResult = doc.Editor.GetEntity("Select object: ");
                if (entityResult.Status != PromptStatus.OK)
                {
                    throw new InvalidOperationException("Invalid selected object.");
                }

                var selectedEntity = transaction.GetObject(entityResult.ObjectId, OpenMode.ForWrite) as Entity;
                if (selectedEntity == null)
                {
                    throw new TransactionException("The selected entity is not found.");
                }

                selectedEntity.ColorIndex = Color.FromColor(System.Drawing.Color.Red).ColorIndex;
                transaction.Commit();
            }
            catch (Exception e)
            {
                doc.Editor.WriteMessage(e.Message + Environment.NewLine);
            }
            finally
            {
                transaction.Dispose();
            }
        }
    }
}
