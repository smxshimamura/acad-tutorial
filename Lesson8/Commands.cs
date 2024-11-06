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

[assembly: CommandClass(typeof(Lesson8.Commands))]
[assembly: ExtensionApplication(typeof(Lesson8.PluginExtension))]

namespace Lesson8
{
    using System.Transactions;

    using Autodesk.AutoCAD.EditorInput;

    /// <summary>
    /// The commands.
    /// </summary>
    public class Commands
    {
        private const string AppName = "ST_Lesson8";

        private const int StringTypeCode = 1000;
        private const int ApplicationTypeCode = 1001;
        private const int ControlTypeCode = 1002;
        private const int DoubleTypeCode = 1040;
        private const int IntegerTypeCode = 1070;

        private const string ControlOpen = "{";
        private const string ControlClose = "}";

        /// <summary>
        /// Creates the circle.
        /// </summary>
        [CommandMethod("Tutorials", "AddXData", "AddXDataId", CommandFlags.Modal)]
        public void AddXData()
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

                doc.Editor.WriteMessage(selectedEntity.GetType().Name + Environment.NewLine);

                var nameResult = GetValueFromEditor(doc.Editor, _ => doc.Editor.GetString("Set name: "));
                var heightResult = GetValueFromEditor(doc.Editor, _ => doc.Editor.GetDouble("Set height: "));
                var ageResult = GetValueFromEditor(doc.Editor, _ => doc.Editor.GetInteger("Set age: "));

                if (IsInvalid(nameResult, heightResult, ageResult))
                {
                    throw new InvalidOperationException("Invalid the specified value.");
                }

                var appTable = transaction.GetObject(database.RegAppTableId, OpenMode.ForWrite) as RegAppTable;
                if (appTable == null)
                {
                    throw new TransactionException("The AppTable is not found.");
                }

                if (!appTable.Has(AppName))
                {
                    var record = new RegAppTableRecord();
                    record.Name = AppName;
                    appTable.Add(record);
                    transaction.AddNewlyCreatedDBObject(record, true);
                }

                selectedEntity.XData = new ResultBuffer(
                    new TypedValue(ApplicationTypeCode, AppName),
                    new TypedValue(ControlTypeCode, ControlOpen),
                    new TypedValue(StringTypeCode, "Name"),
                    new TypedValue(StringTypeCode, nameResult.StringResult),
                    new TypedValue(StringTypeCode, "Height"),
                    new TypedValue(DoubleTypeCode, heightResult.Value),
                    new TypedValue(StringTypeCode, "Age"),
                    new TypedValue(IntegerTypeCode, ageResult.Value),
                    new TypedValue(ControlTypeCode, ControlClose));

                transaction.Commit();
                doc.Editor.WriteMessage(nameof(this.AddXData) + " is completed." + Environment.NewLine);
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

        /// <summary>
        /// Creates the circle.
        /// </summary>
        [CommandMethod("Tutorials", "GetXData", "GetXDataId", CommandFlags.Modal)]
        public void GetXData()
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

                doc.Editor.WriteMessage(selectedEntity.GetType().Name + Environment.NewLine);
                var appTable = transaction.GetObject(database.RegAppTableId, OpenMode.ForRead) as RegAppTable;
                if (appTable == null)
                {
                    throw new TransactionException("The AppTable is not found.");
                }

                if (!appTable.Has(AppName))
                {
                    doc.Editor.WriteMessage("Extended data has not yet been set." + Environment.NewLine);
                }

                var xData = selectedEntity.XData;
                if (xData == null)
                {
                    doc.Editor.WriteMessage("Extended data is not found." + Environment.NewLine);
                    return;
                }

                var readyToRead = false;
                var isValue = false;
                foreach (var data in xData)
                {
                    if (data.Value.Equals(ControlOpen))
                    {
                        readyToRead = true;
                        continue;
                    }

                    if (data.Value.Equals(ControlClose))
                    {
                        break;
                    }

                    if (!readyToRead)
                    {
                        continue;
                    }

                    if (isValue)
                    {
                        doc.Editor.WriteMessage(data.Value + Environment.NewLine);
                    }
                    else
                    {
                        doc.Editor.WriteMessage(data.Value + " = ");
                    }

                    isValue = !isValue;
                }

                doc.Editor.WriteMessage(nameof(this.GetXData) + " is completed." + Environment.NewLine);
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

        private static bool IsInvalid(params PromptResult?[] results)
        {
            if (results.Any())
            {
                return false;
            }

            foreach (var result in results)
            {
                if (result == null)
                {
                    return true;
                }

                if (result.Status != PromptStatus.OK)
                {
                    return true;
                }
            }

            return false;
        }
    }
}
