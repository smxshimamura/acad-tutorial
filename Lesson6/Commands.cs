// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Commands.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the Commands type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

using System.Transactions;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;

using Application = Autodesk.AutoCAD.ApplicationServices.Core.Application;
using Color = Autodesk.AutoCAD.Colors.Color;
using Exception = System.Exception;
using Transaction = Autodesk.AutoCAD.DatabaseServices.Transaction;

[assembly: CommandClass(typeof(Lesson6.Commands))]
[assembly: ExtensionApplication(typeof(Lesson6.PluginExtension))]

namespace Lesson6
{
    /// <summary>
    /// The commands.
    /// </summary>
    public class Commands
    {
        private const string ModelSpace = "*MODEL_SPACE";
        private const string LayerName = "Training";

        /// <summary>
        /// Execute AttachLayer command.
        /// </summary>
        [CommandMethod("Tutorials", "AttachLayer", "AttachLayerId", CommandFlags.Modal)]
        public void AttachLayer()
        {
            var doc = Application.DocumentManager.MdiActiveDocument;
            var database = HostApplicationServices.WorkingDatabase;
            var transaction = database.TransactionManager.StartTransaction();
            try
            {
                var layer = CreateLayer(database, transaction);
                if (layer == null)
                {
                    throw new TransactionException("Failed to create layer.");
                }

                var circle = CreateCircle(database, transaction, doc.Editor);
                circle.SetLayerId(layer.Id, false);

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

        private static LayerTableRecord? CreateLayer(Database database, Transaction transaction)
        {
            var layerTable = transaction.GetObject(database.LayerTableId, OpenMode.ForWrite) as LayerTable;
            if (layerTable == null)
            {
                throw new TransactionException("The LayerTable is not found.");
            }

            if (layerTable.Has(LayerName))
            {
                return transaction.GetObject(layerTable[LayerName], OpenMode.ForWrite) as LayerTableRecord;
            }

            var record = new LayerTableRecord();
            record.Name = LayerName;
            record.Color = Color.FromColor(System.Drawing.Color.Yellow);
            layerTable.Add(record);
            transaction.AddNewlyCreatedDBObject(record, true);

            return record;
        }

        private static Circle CreateCircle(Database database, Transaction transaction, Editor editor)
        {
            var point = GetValueFromEditor(editor, _ => editor.GetPoint("Set the center point of a circle: ").Value);
            var vector = new Vector3d(0.0, 0.0, 1.0);
            var radius = GetValueFromEditor(editor, _ => editor.GetDistance("Set the radius of a circle: ").Value);

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

            return circleObject;
        }

        private static T GetValueFromEditor<T>(Editor editor, Func<Editor, T> method)
        {
            var result = method(editor);
            editor.WriteMessage(result + Environment.NewLine);
            return result;
        }
    }
}
