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

[assembly: CommandClass(typeof(Lesson5.Commands))]
[assembly: ExtensionApplication(typeof(Lesson5.PluginExtension))]

namespace Lesson5
{
    using System.Transactions;
    using System.Windows.Media;

    using Autodesk.AutoCAD.Windows;

    using Lesson5.Views;

    /// <summary>
    /// The commands.
    /// </summary>
    public class Commands
    {
        private const int PaletteWidth = 200;
        private const int PaletteHeight = 400;

        /// <summary>
        /// The key name of model space.
        /// </summary>
        public const string ModelSpace = "*MODEL_SPACE";

        /// <summary>
        /// Creates the circle.
        /// </summary>
        [CommandMethod("Tutorials", "ShowPalette", "ShowPaletteId", CommandFlags.Modal)]
        public void ShowPalette()
        {
            var database = HostApplicationServices.WorkingDatabase;
            var transaction = database.TransactionManager.StartTransaction();
            try
            {
                Show(new PaletteView());
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

        private static void Show(Visual visual)
        {
            var paletteSet = new PaletteSet("Lesson5");
            paletteSet.AddVisual("Character Style Settings", visual);
            paletteSet.Size = new Size(PaletteWidth, PaletteHeight);

            paletteSet.Visible = true;
        }
    }
}
