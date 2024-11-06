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

[assembly: CommandClass(typeof(Lesson2.Commands))]
[assembly: ExtensionApplication(typeof(Lesson2.PluginExtension))]

namespace Lesson2
{
    using System.Transactions;

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
        /// Execute CreateCircle command.
        /// </summary>
        [CommandMethod("Tutorials", "CreateCircle", "CreateCircleId", CommandFlags.Modal)]
        public void CreateCircle()
        {
            var database = HostApplicationServices.WorkingDatabase;
            var transaction = database.TransactionManager.StartTransaction();
            try
            {
                var centralPoint = new Point3d(50.0, 50.0, 0.0);
                var vector = new Vector3d(0.0, 0.0, 1.0);
                var radius = 50.0;

                var circleObject = new Circle(centralPoint, vector, radius);

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
                var doc = Application.DocumentManager.MdiActiveDocument;
                if (doc != null)
                {
                    doc.Editor.WriteMessage(e.Message + Environment.NewLine);
                }
            }
            finally
            {
                transaction.Dispose();
            }
        }
    }
}
