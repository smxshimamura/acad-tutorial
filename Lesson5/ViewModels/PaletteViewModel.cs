// --------------------------------------------------------------------------------------------------------------------
// <copyright file="PaletteViewModel.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the ViewModel.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

namespace Lesson5.ViewModels
{
    using System.Windows.Input;

    using Autodesk.AutoCAD.DatabaseServices;

    using Application = Autodesk.AutoCAD.ApplicationServices.Core.Application;

    /// <summary>
    ///  The view of palette.
    /// </summary>
    /// <seealso cref="Lesson5.ViewModels.BindableObject" />
    internal class PaletteViewModel : BindableObject
    {
        private string styleName = string.Empty;

        /// <summary>
        /// Initializes a new instance of the <see cref="PaletteViewModel"/> class.
        /// </summary>
        public PaletteViewModel()
        {
            this.CreateCommand = new RelayCommand(
                () =>
                    {
                        if (string.IsNullOrWhiteSpace(this.StyleName))
                        {
                            return;
                        }

                        this.RegisterTextStyle();
                    });
        }

        /// <summary>
        /// Gets or sets the name of the style.
        /// </summary>
        /// <value>
        /// The name of the style.
        /// </value>
        public string StyleName
        {
            get => this.styleName;
            set => this.SetField(ref this.styleName, value);
        }

        /// <summary>
        /// Gets the create command.
        /// </summary>
        /// <value>
        /// The create command.
        /// </value>
        public ICommand CreateCommand { get; }

        private void RegisterTextStyle()
        {
            var doc = Application.DocumentManager.MdiActiveDocument;
            var database = HostApplicationServices.WorkingDatabase;

            var documentLock = doc.LockDocument();
            var transaction = database.TransactionManager.StartTransaction();
            try
            {
                var textStyleTable =
                    transaction.GetObject(database.TextStyleTableId, OpenMode.ForWrite) as TextStyleTable;
                if (textStyleTable == null)
                {
                    return;
                }

                if (textStyleTable.Has(this.StyleName))
                {
                    doc.Editor.WriteMessage(
                        this.StyleName + " is already exists." + Environment.NewLine);

                    return;
                }

                var textStyleRecord = new TextStyleTableRecord
                                          {
                                              Name = this.StyleName,
                                              FileName = "romans.shx",
                                              BigFontFileName = "extfont2.shx"
                                          };
                textStyleTable.Add(textStyleRecord);
                transaction.AddNewlyCreatedDBObject(textStyleRecord, true);
                transaction.Commit();
                doc.Editor.WriteMessage(this.StyleName + " has been added." + Environment.NewLine);
            }
            finally
            {
                transaction.Dispose();
                documentLock.Dispose();
            }
        }

    }
}
