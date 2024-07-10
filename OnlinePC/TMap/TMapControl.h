#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace ChartFX::WinForms;
using namespace ChartFX::WinForms::Annotation;


namespace TMap {

	/// <summary>
	/// TMapコントロールの概要
	/// </summary>
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	public ref class TMapControl : public System::Windows::Forms::UserControl
	{
	public:
		TMapControl(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//

			myData = gcnew array<int,2>(22,121);
			for(int i = 0; i < 22; i++ )
				for(int j = 0; j < 121; j++ )
					myData[i,j] = 0;
		}

		System::Void setData(int *pData)
		{
			for(int i = 0; i < 22; i++ )
				for(int j = 0; j < 121; j++ )
					myData[i,j] = pData[i*121+j];
		}

	protected: 
		array<int,2>	^myData;

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~TMapControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: ChartFX::WinForms::Chart^  chart1;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Panel^  panel15;
	private: System::Windows::Forms::Panel^  panel8;
	private: System::Windows::Forms::Panel^  panel7;
	private: System::Windows::Forms::Panel^  panel9;
	private: System::Windows::Forms::Panel^  panel6;
	private: System::Windows::Forms::Panel^  panel10;
	private: System::Windows::Forms::Panel^  panel5;
	private: System::Windows::Forms::Panel^  panel11;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Panel^  panel12;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Panel^  panel13;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel14;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->chart1 = (gcnew ChartFX::WinForms::Chart());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel15 = (gcnew System::Windows::Forms::Panel());
			this->panel8 = (gcnew System::Windows::Forms::Panel());
			this->panel7 = (gcnew System::Windows::Forms::Panel());
			this->panel9 = (gcnew System::Windows::Forms::Panel());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->panel10 = (gcnew System::Windows::Forms::Panel());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->panel11 = (gcnew System::Windows::Forms::Panel());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->panel12 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel13 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel14 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			this->chart1->Location = System::Drawing::Point(65, 119);
			this->chart1->Margin = System::Windows::Forms::Padding(4);
			this->chart1->Name = L"chart1";
			this->chart1->Size = System::Drawing::Size(1111, 676);
			this->chart1->TabIndex = 2;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(1210, 800);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(29, 12);
			this->label15->TabIndex = 68;
			this->label15->Text = L"0-20";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(1210, 750);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(35, 12);
			this->label14->TabIndex = 67;
			this->label14->Text = L"20-40";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(1210, 700);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(35, 12);
			this->label13->TabIndex = 66;
			this->label13->Text = L"40-60";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(1210, 650);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(35, 12);
			this->label12->TabIndex = 65;
			this->label12->Text = L"60-80";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(1210, 600);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(41, 12);
			this->label11->TabIndex = 64;
			this->label11->Text = L"80-100";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(1210, 550);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(47, 12);
			this->label10->TabIndex = 63;
			this->label10->Text = L"100-120";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(1210, 500);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(47, 12);
			this->label9->TabIndex = 62;
			this->label9->Text = L"120-140";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(1210, 450);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(47, 12);
			this->label8->TabIndex = 61;
			this->label8->Text = L"140-160";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(1210, 400);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(47, 12);
			this->label7->TabIndex = 60;
			this->label7->Text = L"160-180";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(1210, 350);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(47, 12);
			this->label6->TabIndex = 59;
			this->label6->Text = L"180-200";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(1210, 300);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(47, 12);
			this->label5->TabIndex = 58;
			this->label5->Text = L"200-220";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(1210, 250);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(47, 12);
			this->label4->TabIndex = 57;
			this->label4->Text = L"220-240";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1210, 200);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(47, 12);
			this->label3->TabIndex = 56;
			this->label3->Text = L"240-260";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(1210, 150);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(47, 12);
			this->label2->TabIndex = 55;
			this->label2->Text = L"260-280";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1210, 100);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 12);
			this->label1->TabIndex = 54;
			this->label1->Text = L"280-300";
			// 
			// panel15
			// 
			this->panel15->BackColor = System::Drawing::Color::Red;
			this->panel15->Location = System::Drawing::Point(1194, 783);
			this->panel15->Name = L"panel15";
			this->panel15->Size = System::Drawing::Size(10, 49);
			this->panel15->TabIndex = 53;
			// 
			// panel8
			// 
			this->panel8->BackColor = System::Drawing::Color::Red;
			this->panel8->Location = System::Drawing::Point(1194, 433);
			this->panel8->Name = L"panel8";
			this->panel8->Size = System::Drawing::Size(10, 49);
			this->panel8->TabIndex = 49;
			// 
			// panel7
			// 
			this->panel7->BackColor = System::Drawing::Color::Red;
			this->panel7->Location = System::Drawing::Point(1194, 383);
			this->panel7->Name = L"panel7";
			this->panel7->Size = System::Drawing::Size(10, 49);
			this->panel7->TabIndex = 43;
			// 
			// panel9
			// 
			this->panel9->BackColor = System::Drawing::Color::Red;
			this->panel9->Location = System::Drawing::Point(1194, 483);
			this->panel9->Name = L"panel9";
			this->panel9->Size = System::Drawing::Size(10, 49);
			this->panel9->TabIndex = 50;
			// 
			// panel6
			// 
			this->panel6->BackColor = System::Drawing::Color::Red;
			this->panel6->Location = System::Drawing::Point(1194, 333);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(10, 49);
			this->panel6->TabIndex = 44;
			// 
			// panel10
			// 
			this->panel10->BackColor = System::Drawing::Color::Red;
			this->panel10->Location = System::Drawing::Point(1194, 533);
			this->panel10->Name = L"panel10";
			this->panel10->Size = System::Drawing::Size(10, 49);
			this->panel10->TabIndex = 51;
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::Red;
			this->panel5->Location = System::Drawing::Point(1194, 283);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(10, 49);
			this->panel5->TabIndex = 41;
			// 
			// panel11
			// 
			this->panel11->BackColor = System::Drawing::Color::Red;
			this->panel11->Location = System::Drawing::Point(1194, 583);
			this->panel11->Name = L"panel11";
			this->panel11->Size = System::Drawing::Size(10, 49);
			this->panel11->TabIndex = 52;
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::Red;
			this->panel4->Location = System::Drawing::Point(1194, 233);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(10, 49);
			this->panel4->TabIndex = 42;
			// 
			// panel12
			// 
			this->panel12->BackColor = System::Drawing::Color::Red;
			this->panel12->Location = System::Drawing::Point(1194, 633);
			this->panel12->Name = L"panel12";
			this->panel12->Size = System::Drawing::Size(10, 49);
			this->panel12->TabIndex = 48;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::Red;
			this->panel3->Location = System::Drawing::Point(1194, 183);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(10, 49);
			this->panel3->TabIndex = 45;
			// 
			// panel13
			// 
			this->panel13->BackColor = System::Drawing::Color::Red;
			this->panel13->Location = System::Drawing::Point(1194, 683);
			this->panel13->Name = L"panel13";
			this->panel13->Size = System::Drawing::Size(10, 49);
			this->panel13->TabIndex = 47;
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Red;
			this->panel2->Location = System::Drawing::Point(1194, 133);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(10, 49);
			this->panel2->TabIndex = 40;
			// 
			// panel14
			// 
			this->panel14->BackColor = System::Drawing::Color::Red;
			this->panel14->Location = System::Drawing::Point(1194, 733);
			this->panel14->Name = L"panel14";
			this->panel14->Size = System::Drawing::Size(10, 49);
			this->panel14->TabIndex = 46;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Red;
			this->panel1->Location = System::Drawing::Point(1194, 83);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(10, 49);
			this->panel1->TabIndex = 39;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 500;
			this->timer1->Tick += gcnew System::EventHandler(this, &TMapControl::timer1_Tick);
			// 
			// TMapControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->panel15);
			this->Controls->Add(this->panel8);
			this->Controls->Add(this->panel7);
			this->Controls->Add(this->panel9);
			this->Controls->Add(this->panel6);
			this->Controls->Add(this->panel10);
			this->Controls->Add(this->panel5);
			this->Controls->Add(this->panel11);
			this->Controls->Add(this->panel4);
			this->Controls->Add(this->panel12);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel13);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel14);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->chart1);
			this->Name = L"TMapControl";
			this->Size = System::Drawing::Size(1280, 925);
			this->Load += gcnew System::EventHandler(this, &TMapControl::TMapControl_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	// 画面の初期化
	private: System::Void TMapControl_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		// 基本設定
		this->chart1->Gallery = Gallery::Contour;
		this->chart1->AllSeries->MultipleColors = true;
		this->chart1->AllSeries->FillMode = FillMode::Solid; 
		this->chart1->Data->Series = 22;
		this->chart1->Data->Points = 121;
		this->chart1->LegendBox->Visible = false;
		//this->chart1->Antialiasing = false;

		String	^str;

		// 項目名
//		this->chart1->AxisX->Step = 1;
		//for( int i = 0; i < 22; i++ )
		//{
		//	this->chart1->AxisY->Labels[i] = "";
		//}
		//this->chart1->AxisX->LabelAngle = 0;
		//this->chart1->AxisX->Position = AxisPosition::Near;
//		this->chart1->AxisX->Font = gcnew System::Drawing::Font("MS UI Gothic", 9);
		this->chart1->AxisX->Visible = false;
		this->chart1->AxisY->Visible = false;

		// 色の設定
		array<Color>^ myColor = gcnew array<Color>(15);
		myColor[ 0] = Color::FromArgb( 55, 55,255);
		myColor[ 1] = Color::FromArgb( 55,155,255);
		myColor[ 2] = Color::FromArgb( 55,255,255);
		myColor[ 3] = Color::FromArgb( 55,255,155);
		myColor[ 4] = Color::FromArgb( 55,255, 55);
		myColor[ 5] = Color::FromArgb(155,255, 55);
		myColor[ 6] = Color::FromArgb(255,255, 55);
		myColor[ 7] = Color::FromArgb(255,230, 55);
		myColor[ 8] = Color::FromArgb(255,205, 55);
		myColor[ 9] = Color::FromArgb(255,180, 55);
		myColor[10] = Color::FromArgb(255,155, 55);
		myColor[11] = Color::FromArgb(255,130, 55);
		myColor[12] = Color::FromArgb(255,105, 55);
		myColor[13] = Color::FromArgb(255, 80, 55);
		myColor[14] = Color::FromArgb(255, 55, 55);
		Galleries::Surface ^mySurface = (Galleries::Surface ^)this->chart1->GalleryAttributes;
		mySurface->Step = 20;
		mySurface->Colors = myColor;
		mySurface->ShowGridLines = true;
				
		// パネルの設定
		panel1->BackColor = myColor[14];
		panel2->BackColor = myColor[13];
		panel3->BackColor = myColor[12];
		panel4->BackColor = myColor[11];
		panel5->BackColor = myColor[10];
		panel6->BackColor = myColor[ 9];
		panel7->BackColor = myColor[ 8];
		panel8->BackColor = myColor[ 7];
		panel9->BackColor = myColor[ 6];
		panel10->BackColor = myColor[ 5];
		panel11->BackColor = myColor[ 4];
		panel12->BackColor = myColor[ 3];
		panel13->BackColor = myColor[ 2];
		panel14->BackColor = myColor[ 1];
		panel15->BackColor = myColor[ 0];

		// 3D Setting
		this->chart1->View3D->Enabled = false;

		//Constrains
		this->chart1->ToolTips = false;

		///////////////////////////////////////////////////////////////
		// Surface

		this->chart1->PlotAreaMargin->Left = 30;
		this->chart1->PlotAreaMargin->Right = 30;
		this->chart1->PlotAreaMargin->Top = 0;
		this->chart1->PlotAreaMargin->Bottom = 20;

		// color
		this->chart1->AxisY->Style &= ~AxisStyles::BreakZero;
		this->chart1->BackColor = Color::White;
		this->chart1->AxisY->Step = 20;
		this->chart1->AxisY->Min = 0;
		this->chart1->AxisY->Max = 300;
		this->chart1->AxisY->Title->Text = "";

		Annotations ^annots = gcnew Annotations();     
		this->chart1->Extensions->Add(annots);

		// テキスト描画
		AnnotationText ^text1 = gcnew AnnotationText();     
		text1->Text = "Ｄ";     
		text1->Top = 150;     
		text1->Left = 140; 
		text1->Border->Color = Color::White;
		text1->Font = gcnew System::Drawing::Font("MS UI Gothic", 20);
		annots->List->Add(text1);     
		AnnotationText ^text2 = gcnew AnnotationText();     
		text2->Text = "Ｃ";     
		text2->Top = 150;     
		text2->Left = 390; 
		text2->Border->Color = Color::White;
		text2->Font = gcnew System::Drawing::Font("MS UI Gothic", 20);
		annots->List->Add(text2);     
		AnnotationText ^text3 = gcnew AnnotationText();     
		text3->Text = "Ｂ";     
		text3->Top = 150;     
		text3->Left = 680; 
		text3->Border->Color = Color::White;
		text3->Font = gcnew System::Drawing::Font("MS UI Gothic", 20);
		annots->List->Add(text3);     
		AnnotationText ^text4 = gcnew AnnotationText();     
		text4->Text = "Ａ";     
		text4->Top = 150;     
		text4->Left = 930; 
		text4->Border->Color = Color::White;
		text4->Font = gcnew System::Drawing::Font("MS UI Gothic", 20);
		annots->List->Add(text4); 

		// 直線の描画
		AnnotationArrow ^arrow1 = gcnew AnnotationArrow();
		arrow1->Border->Color = Color::Black;
		arrow1->Color = Color::Black;
		arrow1->Border->Width = 3;
		System::Drawing::Drawing2D::AdjustableArrowCap ^endc1 = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(1, 1); 
		arrow1->EndCap = endc1;
		arrow1->Attach(28, 79, 28, 221);
		annots->List->Add(arrow1);
		AnnotationArrow ^arrow2 = gcnew AnnotationArrow();
		arrow2->Border->Color = Color::Black;
		arrow2->Color = Color::Black;
		arrow2->Border->Width = 3;
		System::Drawing::Drawing2D::AdjustableArrowCap ^endc2 = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(1, 1); 
		arrow2->EndCap = endc2;
		arrow2->Attach(61, 79, 61, 221);
		annots->List->Add(arrow2);
		AnnotationArrow ^arrow3 = gcnew AnnotationArrow();
		arrow3->Border->Color = Color::Black;
		arrow3->Color = Color::Black;
		arrow3->Border->Width = 3;
		System::Drawing::Drawing2D::AdjustableArrowCap ^endc3 = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(1, 1); 
		arrow3->EndCap = endc3;
		arrow3->Attach(94, 79, 94, 221);
		annots->List->Add(arrow3);
	}

	// タイマー処理
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
	{
		for(int i = 0; i < 22; i++ )
			for( int j = 0; j < 121; j++ )
			{
				this->chart1->Data[i,j] = myData[21-i,j]/10.0;
			}
	}
};
}
