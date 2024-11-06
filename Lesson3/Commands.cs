// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Commands.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the Commands type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;
using Application = Autodesk.AutoCAD.ApplicationServices.Core.Application;
using Exception = System.Exception;

[assembly: CommandClass(typeof(Lesson3.Commands))]
[assembly: ExtensionApplication(typeof(Lesson3.PluginExtension))]

namespace Lesson3
{
    using System.Transactions;

    using Autodesk.AutoCAD.EditorInput;

    /// <summary>
    /// The commands.
    /// </summary>
    public class Commands
    {
        /// <summary>
        /// The key name of model space.
        /// </summary>
        public const string ModelSpace = "*MODEL_SPACE";

        /// <summary>
        /// Execute CreateCircle2 command.
        /// </summary>
        [CommandMethod("Tutorials", "CreateCircle2", "CreateCircle2Id", CommandFlags.Modal)]
        public void CreateCircle2()
        {
            var doc = Application.DocumentManager.MdiActiveDocument;
            if (doc == null)
            {
                throw new InvalidOperationException("The document is not exists.");
            }

            var database = HostApplicationServices.WorkingDatabase;
            var transaction = database.TransactionManager.StartTransaction();
            try
            {
                var point = GetValueFromEditor(doc.Editor, editor => editor.GetPoint("Set the center point of a circle: ").Value);
                var vector = new Vector3d(0.0, 0.0, 1.0);
                var radius = GetValueFromEditor(doc.Editor, editor => editor.GetDistance("Set the radius of a circle: ").Value);

                var circleObject = new Circle(point, vector, radius);

                var blockTable = transaction.GetObject(database.BlockTableId, OpenMode.ForRead) as BlockTable;
                if (blockTable == null)
                {
                    throw new TransactionException("The BlockTable is not found.");
                }

                var record = transaction.GetObject(blockTable[ModelSpace], OpenMode.ForWrite) as BlockTableRecord;
                if (record == null)
                {
                    throw new TransactionException("The BlockTableRecord is not found.");
                }

                record.AppendEntity(circleObject);
                transaction.AddNewlyCreatedDBObject(circleObject, true);

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

        private static T GetValueFromEditor<T>(Editor editor, Func<Editor, T> method)
        {
            var result = method(editor);
            editor.WriteMessage(result + Environment.NewLine);
            return result;
        }
    }
}
